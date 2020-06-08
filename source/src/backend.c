#include <inttypes.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <kcgi.h>
#include <kcgijson.h>
#include "backend.h"
#include <stdlib.h>
#include <err.h>

#include <sqlbox.h>
#include "db.h"

struct	ort {
	/* Hidden database connection */
	struct sqlbox *db;
};

typedef struct ort bvrs_ctxt;

status_t
open_db(const char *dbname, bvrs_ctxt_t **ctxt)
{
    bvrs_ctxt_t *lctxt;

    lctxt = db_open_logging(dbname, NULL, warnx, NULL);
    if (NULL == lctxt) {
        return ERROR;
    }

    *ctxt = lctxt;
    return OK;
}

status_t
close_db(bvrs_ctxt_t *ctxt)
{
    db_close(ctxt);
    return OK;
}

status_t
flush_voter_sessions(bvrs_ctxt_t *ctxt, time_t voter_cutoff)
{
    status_t ok = OK;
    struct voterupdatesession_q *voter_sessions =
        db_voterupdatesession_list_byctime(ctxt, voter_cutoff);
    struct voterupdatesession *vsession;
    TAILQ_FOREACH(vsession, voter_sessions, _entries) {
        int del = db_voterupdatesession_delete_votersession(ctxt,
                                                            vsession->id,
                                                            vsession->token);
        if (!del) {
            ok = ERROR;
        }
    }
    db_voterupdatesession_freeq(voter_sessions);

    return ok;
}

status_t
flush_official_sessions(bvrs_ctxt_t *ctxt, time_t official_cutoff)
{
    status_t ok = OK;
    struct electionofficialsession_q *official_sessions =
        db_electionofficialsession_list_byctime(ctxt, official_cutoff);
    struct electionofficialsession *osession;
    TAILQ_FOREACH(osession, official_sessions, _entries) {
        int del = db_electionofficialsession_delete_officialsession(ctxt,
                                                                    osession->id,
                                                                    osession->token);
        if (!del) {
            ok = ERROR;
        }
    }
    db_electionofficialsession_freeq(official_sessions);

    return ok;
}

status_t
flush_old_sessions(bvrs_ctxt_t *ctxt,
                   time_t voter_session_length,
                   time_t official_session_length)
{
    time_t now             = time(NULL);
    /* assuming these won't overflow... */
    time_t voter_cutoff    = now - voter_session_length;
    time_t official_cutoff = now - official_session_length;

    status_t del_voters    = flush_voter_sessions(ctxt, voter_cutoff);
    status_t del_officials = flush_official_sessions(ctxt, voter_cutoff);

    return (del_voters == OK && del_officials == OK) ? OK : ERROR;
}

/* Each of the following corresponds to a command or query from the Lando
 * specification of the BVRS, and hence forms the backend server API
 */

status_t
new_session_info(int64_t *the_token, time_t *the_time)
{
    *the_time = time(NULL);
    *the_token = random();

    return OK;
}

// Voter Operations
status_t
new_voter_session(bvrs_ctxt_t *ctxt,
                  const char *lastname,
                  const char *givenname,
                  const char *resaddr,
                  const char *resaddr2,
                  const char *reszip,
                  const char *resstate,
                  const char *mailaddr,
                  const char *mailaddr2,
                  const char *mailzip,
                  const char *mailstate,
                  time_t birthdate,
                  const void *idinfo,
                  size_t idinfo_sz,
                  int64_t confidential,
                  struct voter **the_voter,
                  int64_t *the_session_id,
                  int64_t *the_token)
{
    struct voter *a_voter;
    status_t retstatus = ERROR;

    a_voter = db_voter_get_registrationforupdate(ctxt,
                                                 lastname,
                                                 givenname,
                                                 resaddr,
                                                 mailaddr,
                                                 birthdate,
                                                 idinfo_sz,
                                                 idinfo,
                                                 confidential);
    if (NULL != a_voter) {
        time_t ctime;
        int64_t a_token;
        int64_t a_session_id;

        if (OK == new_session_info(&a_token, &ctime)) {

            a_session_id = db_voterupdatesession_insert(ctxt,
                                                        a_voter->id,
                                                        a_token,
                                                        ctime);
            if (a_session_id > 0) {
                *the_session_id = a_session_id;
                *the_token = a_token;
                *the_voter = a_voter;
                retstatus = OK;
            }
        } else {
            retstatus = ERROR;
        }
    } else {
        retstatus = NOT_FOUND;
    }

    return retstatus;
}

status_t
lookup_voter_session(bvrs_ctxt_t *ctxt,
                     int64_t the_session_id,
                     int64_t the_token,
                     int64_t *voter_id)
{
    status_t ret = ERROR;
    struct voterupdatesession *session;
    session = db_voterupdatesession_get_updatecreds(ctxt, the_session_id, the_token);
    if (NULL != session) {
        *voter_id = session->voterid;
        db_voterupdatesession_free(session);
        ret = OK;
    } else {
        ret = NOT_FOUND;
    }

    return ret;
}

status_t
end_voter_session(bvrs_ctxt_t *ctxt,
                  int64_t session_id,
                  int64_t token)
{
    int status = db_voterupdatesession_delete_votersession(ctxt, session_id, token);
    if (status == 0) {
        return ERROR;
    }

    return OK;
}

status_t
lookup_voter_information(bvrs_ctxt_t *ctxt,
                         const char *lastname,
                         const char *givenname,
                         time_t birthdate,
                         int64_t confidential,
                         struct voter_q **voters)
{
    struct voter_q *some_voters;

    some_voters = db_voter_list_registration(ctxt,
                                             lastname,
                                             givenname,
                                             birthdate,
                                             confidential);
    if (NULL != some_voters) {
        *voters = some_voters;
    } else {
        *voters = NULL;
    }
    return OK;
}

status_t
register_voter(bvrs_ctxt_t *ctxt,
               const char *lastname,
               const char *givennames,
               const char *resaddr,
               const char *resaddr2,
               const char *reszip,
               const char *resstate,
               const char *mailaddr,
               const char *mailaddr2,
               const char *mailzip,
               const char *mailstate,
               const char *registeredparty,
               time_t birthdate,
               const void *idinfo,
               size_t idinfo_sz,
               int64_t confidential,
               int64_t *out_id)
{
    time_t now = time(NULL);
/*
* 	v1: lastname
 * 	v2: givennames
 * 	v3: resaddress
 * 	v4: resaddress2
 * 	v5: reszip
 * 	v6: resstate
 * 	v7: mailaddress
 * 	v8: mailaddress2
 * 	v9: mailzip
 * 	v10: mailstate
 * 	v11: registeredparty
 * 	v12: birthdate
 * 	v13: idinfo
 * 	v14: status
 * 	v15: initialregtime
 * 	v16: lastupdatetime
 * 	v17: confidential
 */
    int64_t id = db_voter_insert(ctxt,
                                 lastname,
                                 givennames,
                                 resaddr,
                                 resaddr2,
                                 reszip,
                                 resstate,
                                 mailaddr,
                                 mailaddr2,
                                 mailzip,
                                 mailstate,
                                 registeredparty,
                                 birthdate,
                                 idinfo_sz,
                                 idinfo,
                                 REGSTATUS_PENDINGREVIEW,
                                 now,
                                 now,
                                 confidential);
    if (id < 0) {
        return CONSTRAINT_VIOLATION;
    } else {
        *out_id = id;
        return OK;
    }
}

status_t
update_voter_status(bvrs_ctxt_t *ctxt,
                    int64_t voter_id,
                    enum regstatus new_status)
{
    int update_ok = db_voter_update_status(ctxt, new_status, time(NULL), voter_id);
    if (update_ok) {
        return OK;
    } else {
        return ERROR;
    }

}

status_t
update_voter_information(bvrs_ctxt_t *ctxt,
                         int64_t voter_id,
                         const char *lastname,
                         const char *givennames,
                         const char *resaddr,
                         const char *resaddr2,
                         const char *reszip,
                         const char *resstate,
                         const char *mailaddr,
                         const char *mailaddr2,
                         const char *mailzip,
                         const char *mailstate,
                         const char *registeredparty,
                         time_t birthdate,
                         const void *idinfo,
                         size_t idinfo_sz,
                         enum regstatus status,
                         int64_t confidential)
{
    int updateok = db_voter_update_info(ctxt,
                                        lastname,
                                        givennames,
                                        resaddr,
                                        resaddr2,
                                        reszip,
                                        resstate,
                                        mailaddr,
                                        mailaddr2,
                                        mailzip,
                                        mailstate,
                                        registeredparty,
                                        birthdate,
                                        idinfo_sz,
                                        idinfo,
                                        status,
                                        time(NULL),
                                        confidential,
                                        voter_id);
    if (0 == updateok) {
        return ERROR;
    }

    return OK;
}

// @todo Not clear what the actual input to this will be:
// raw SQL?
status_t
query_voter_database(bvrs_ctxt_t *ctxt,
                     const char *lastname,
                     const char *givennames,
                     const char *resaddress,
                     const char *mailaddress,
                     time_t birthdate_lower,
                     time_t birthdate_upper,
                     struct voter_q **the_voters)
{
    struct voter_q *some_voters;
    some_voters = db_voter_list_officialsearch(ctxt,
                                               lastname,
                                               givennames,
                                               resaddress,
                                               mailaddress,
                                               birthdate_lower,
                                               birthdate_upper);

    if (NULL != some_voters) {
        *the_voters = some_voters;
        return OK;
    }

    return ERROR;
}

// Election Official Operations
status_t
new_official_session(bvrs_ctxt_t *ctxt,
                     const char *username,
                     const char *password,
                     int64_t *session_id,
                     int64_t *token)
{
    struct electionofficial *official;
    status_t retstatus = ERROR;

    official = db_electionofficial_get_credentials(ctxt, username, password);

    if (NULL != official) {
        time_t ctime;
        int64_t a_token;
        int64_t a_session_id;

        if (OK == new_session_info(&a_token, &ctime)) {

            a_session_id = db_electionofficialsession_insert(ctxt, official->id, a_token, ctime);

            if (a_session_id > 0) {
                *session_id = a_session_id;
                *token = a_token;
                retstatus = OK;
            }
        } else {
            retstatus = ERROR;
        }
        db_electionofficial_free(official);
    } else {
        return NOT_FOUND;
    }

    return retstatus;
}

status_t
lookup_official_session(bvrs_ctxt_t *ctx,
                        int64_t *session_id,
                        int64_t *token)
{
    status_t ret = ERROR;
    struct electionofficialsession *session;
    session = db_electionofficialsession_get_officialcreds(ctx, *session_id, *token);
    if (NULL != session) {
        db_electionofficialsession_free(session);
        ret = OK;
    } else {
        ret = NOT_FOUND;
    }

    return ret;
}


status_t
end_official_session(bvrs_ctxt_t *ctxt,
                     int64_t the_session_id,
                     int64_t the_token)
{
    int status = db_electionofficialsession_delete_officialsession(ctxt,
                                                                   the_session_id,
                                                                   the_token);
    if (status == 0) {
        return ERROR;
    }

    return OK;
}

status_t official_query(bvrs_ctxt_t *ctxt,
                        const char *field_name,
                        const char *field_contains,
                        bool invert_contains,
                        const char *date_field,
                        time_t date_from,
                        time_t date_thru,
                        bool invert_date_selection,
                        tristate_t active_status,
                        tristate_t updated_status)
{
    int max_parms = 9;
    int parmcnt = 0;
	struct voter *p;
	struct voter_q *q;
    struct sqlbox* db = ctxt->db;
	const struct sqlbox_parmset *res;
    char *where_clause = "WHERE ";
    char *tmp, *fragment;
	struct sqlbox_parm parms[max_parms];
    memset(parms, 0, sizeof(parms));

    // int sqlite3_open(
    //     const char *filename,   /* Database filename (UTF-8) */
    //     sqlite3 **ppDb          /* OUT: SQLite db handle */
    // );

    if(field_name != NULL && strlen(field_name) > 0 
      && field_contains != NULL && strlen(field_contains) > 0) {

        if(invert_contains) {
            tmp = "%s LIKE%%?%% ";
        } else {
            tmp = "NOT %s LIKE%%?%% ";
        }
        sprintf(fragment, tmp, field_name);
        fragment = (char *)calloc(strlen(tmp), sizeof(char));
        sprintf(fragment, tmp, field_name);
        tmp = where_clause;
        where_clause = (char *)calloc(strlen(tmp) + strlen(fragment) + 1, sizeof(char));
        strcat(where_clause, tmp);
        strcat(where_clause, fragment);
        parmcnt++;
        parms[parmcnt].sparm = field_contains;
        parms[parmcnt].type = SQLBOX_PARM_STRING;
    }

    if(date_field != NULL && strlen(date_field) > 0) {
        if(invert_date_selection) {
            fragment = "%s BETWEEN ? AND ? ";
        } else {
            fragment = "%s NOT BETWEEN ? AND ? ";
        }
        tmp = where_clause;
        where_clause = (char *)calloc(strlen(tmp) + strlen(fragment) + 1, sizeof(char));
        strcat(where_clause, tmp);
        strcat(where_clause, fragment);
        parmcnt++;
        parms[parmcnt].iparm = date_from;
        parms[parmcnt].type = SQLBOX_PARM_INT;
        parmcnt++;
        parms[parmcnt].iparm = date_thru;
        parms[parmcnt].type = SQLBOX_PARM_INT;
    }

    if(NOT_DEF != active_status) {
        if(ACTIVE == active_status) {
            fragment = "active = 0 ";
        } else {
            fragment = "active = 1 ";
        }
    }

    // TODO: How do we determine an updated record?

    
    char *stmt;
    sprintf(stmt, "SELECT * FROM voter WHERE %s", where_clause);
    DBG("%s", stmt);

    
	// if (!sqlbox_prepare_bind_async
	//     (db, 0, stmt,
	//      4, parms, SQLBOX_STMT_MULTI))
	// 	exit(EXIT_FAILURE);
	// while ((res = sqlbox_step(db, 0)) != NULL && res->psz) {
	// 	p = malloc(sizeof(struct voter));
	// 	if (p == NULL) {
	// 		perror(NULL);
	// 		exit(EXIT_FAILURE);
	// 	}
	// 	db_voter_fill_r(ctxt, p, res, NULL);
	// 	TAILQ_INSERT_TAIL(q, p, _entries);
	// }
	// if (res == NULL)
	// 	exit(EXIT_FAILURE);
	// if (!sqlbox_finalise(db, 0))
	// 	exit(EXIT_FAILURE);
	//return q;
    return ERROR;
}