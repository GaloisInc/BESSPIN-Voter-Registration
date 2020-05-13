#ifndef __BACKEND__
#define __BACKEND__

#include <unistd.h>
#include <stdint.h>
#include <time.h>
#include "db.h"

typedef enum status {
  ERROR,
  OK
} status_t;

typedef struct ort bvrs_ctxt_t;

/* Each of the following corresponds to a command or query from the Lando
 * specification of the BVRS, and hence forms the backend server API
 */

// Voter Operations
status_t
new_voter_session(bvrs_ctxt_t *ctxt,
                  struct voter *the_voter,
                  struct voterupdatesession **new_session);

status_t
end_voter_session(bvrs_ctxt_t *ctxt,
                  struct voterupdatesession *the_session);

status_t
lookup_voter_information(bvrs_ctxt_t *ctxt,
                         const char *lastname,
                         const char *givenname,
                         const char *birthdate,
                         int64_t condiential,
                         struct voter **voters,
                         size_t        *n_voters);

status_t
register_voter(bvrs_ctxt_t *ctxt,
               struct voter *the_voter);

status_t
unregister_voter(bvrs_ctxt_t *ctxt,
                 struct voter *the_voter);

status_t
update_voter_information(bvrs_ctxt_t *ctxt,
                         struct voter *the_voter);

// @todo Not clear what the actual input to this will be:
// raw SQL?
status_t
query_voter_database(bvrs_ctxt_t *ctxt,
                     struct voter **the_voters,
                     size_t n_voters);

// Election Official Operations
status_t
new_official_session(bvrs_ctxt_t *ctxt,
                     const char *username,
                     const char *password,
                     struct electionofficialsession **the_session);

status_t
end_official_session(bvrs_ctxt_t *ctxt,
                     struct electionofficialsession *the_session);

#endif //__BACKEND__