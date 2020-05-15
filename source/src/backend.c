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

/* Each of the following corresponds to a command or query from the Lando
 * specification of the BVRS, and hence forms the backend server API
 */

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

        ctime = time(NULL);
        a_token = random();

        a_session_id = db_voterupdatesession_insert(ctxt,
                                                    a_voter->id,
                                                    a_token,
                                                    ctime);
        if (a_session_id > 0) {
            *the_session_id = a_session_id;
            *the_token = a_token;
            *the_voter = a_voter;
            return OK;
        }
    }

    return ERROR;
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
unregister_voter(bvrs_ctxt_t *ctxt, int64_t voter_id)
{
    return ERROR;
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
                     struct voter_q **the_voters)
{
    return ERROR;
}

// Election Official Operations
status_t
new_official_session(bvrs_ctxt_t *ctxt,
                     const char *username,
                     const char *password,
                     struct electionofficialsession **the_session)
{
    return ERROR;
}

status_t
end_official_session(bvrs_ctxt_t *ctxt,
                     struct electionofficialsession *the_session)
{
    return ERROR;
}
