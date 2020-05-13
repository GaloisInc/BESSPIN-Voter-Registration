#include "backend.h"

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
                  struct voterupdatesession **new_session)
{
    return ERROR;
}

status_t
end_voter_session(bvrs_ctxt_t *ctxt,
                  struct voterupdatesession *the_session)
{
    return ERROR;
}

status_t
lookup_voter_information(bvrs_ctxt_t *ctxt,
                         const char *lastname,
                         const char *givenname,
                         time_t birthdate,
                         int64_t condiential,
                         struct voter **voters,
                         size_t        *n_voters)

{
    return ERROR;
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
    return ERROR;
}

status_t
unregister_voter(bvrs_ctxt_t *ctxt,
                 struct voter *the_voter)
{
    return ERROR;
}

status_t
update_voter_information(bvrs_ctxt_t *ctxt,
                         struct voterupdatesession *the_session,
                         char *lastname,
                         char *givennames,
                         char *resaddress,
                         char *mailaddress,
                         char *registeredparty,
                         time_t birthdate,
                         void *idinfo,
                         size_t idinfo_sz,
                         int64_t confidential)
{
    return ERROR;
}

// @todo Not clear what the actual input to this will be:
// raw SQL?
status_t
query_voter_database(bvrs_ctxt_t *ctxt,
                     struct voter **the_voters,
                     size_t n_voters)
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
