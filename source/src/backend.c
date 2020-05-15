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

typedef struct ort bvrs_ctxt;

status_t
open_db(const char *dbname, bvrs_ctxt_t **ctxt)
{
    bvrs_ctxt_t *lctxt;

    lctxt = db_open(dbname);
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
                  const char *mailaddr,
                  time_t birthdate,
                  void *idinfo,
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
    }

    return retstatus;
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
               char *lastname,
               char *givennames,
               char *resaddress,
               char *mailaddress,
               char *registeredparty,
               time_t birthdate,
               void *idinfo,
               size_t idinfo_sz,
               int64_t confidential,
               int64_t *out_id)
{
    time_t now;
    time(&now);
    int64_t id = db_voter_insert(ctxt,
                                 lastname,
                                 givennames,
                                 resaddress,
                                 mailaddress,
                                 registeredparty,
                                 birthdate,
                                 idinfo_sz,
                                 idinfo,
                                 REGSTATUS_ACTIVE,
                                 now,
                                 now,
                                 confidential);
    if (id < 0) {
        return ERROR;
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
                         char *lastname,
                         char *givennames,
                         char *resaddress,
                         char *mailaddress,
                         char *registeredparty,
                         time_t birthdate,
                         void *idinfo,
                         size_t idinfo_sz,
                         enum regstatus status,
                         int64_t confidential)
{
    int updateok = db_voter_update_info(ctxt,
                                        lastname,
                                        givennames,
                                        resaddress,
                                        mailaddress,
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
    }

    return retstatus;
}

status_t
end_official_session(bvrs_ctxt_t *ctxt,
                     struct electionofficialsession *the_session)
{
    return ERROR;
}
