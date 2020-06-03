#ifndef __BACKEND__
#define __BACKEND__

#include <ctype.h>
#include <stdlib.h>
#include <stdarg.h>
#include <unistd.h>
#include <stdint.h>
#include <time.h>
#include <sys/queue.h>

#include <kcgi.h>
#include <kcgijson.h>
#include "db.h"

typedef enum status {
  ERROR,
  CONSTRAINT_VIOLATION,
  NOT_FOUND,
  OK
} status_t;

typedef struct ort bvrs_ctxt_t;

/* Each of the following corresponds to a command or query from the Lando
 * specification of the BVRS, and hence forms the backend server API
 */

/* Open the file dbname and set *ctxt to a valid (opaque) handle on success.
 *
 * Returns
 *   OK on success
 *   ERROR if some internal error occurs
 */
status_t
open_db(const char *dbname, bvrs_ctxt_t **ctxt);

/* Release the context
 *
 * Returns
 *   OK on success
 *   ERROR if some internal error occurs
 */
status_t
close_db(bvrs_ctxt_t *ctxt);

/* Remove all sessions that have expired.
 *
 * A voter session is expired if its ctime was more than voter_session_length
 * seconds ago. An election official session is expired if its ctime was more
 * than official_session_length seconds ago.
 *
 * Returns
 *   OK on success
 *   ERROR if some internal error occurs
 */
status_t
flush_old_sessions(bvrs_ctxt_t *ctxt, time_t voter_session_length, time_t official_session_length);

/* Create a new voter session.
 *
 * The provided information must exactly match a voter in the database. If such
 * a voter exists, return the id and token of the newly created session.
 *
 * Returns
 *   OK if a new session is created
 *   NOT_FOUND if the information does not match an existing voter
 *   ERROR if some internal error occurs
 */
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
                  int64_t *the_token);

/* Explicitly end a session and remove it from the database.
 *
 * Returns
 *   OK on success
 *   ERROR if some internal error occurs
 */
status_t
end_voter_session(bvrs_ctxt_t *ctxt,
                  int64_t the_session_id,
                  int64_t the_token);

/* Lookup a voter session and return the voter_id if found.
 *
 * Returns
 *   OK on success
 *   NOT_FOUND if this does not match a valid session
 *   ERROR if some internal error occurs
 */
status_t
lookup_voter_session(bvrs_ctxt_t *ctxt,
                     int64_t the_session_id,
                     int64_t the_token,
                     int64_t *voter_id);

/* Search for voters by lastname, givenname, birthdate, and whether or not
 * the voter is confidential.
 *
 * Returns
 *   OK on success (including if 0 voters are found)
 *   ERROR if some internal error occurs
 */
status_t
lookup_voter_information(bvrs_ctxt_t *ctxt,
                         const char *lastname,
                         const char *givenname,
                         time_t birthdate,
                         int64_t condiential,
                         struct voter_q **voters);

/* Add a new voter to the database.
 *
 * Returns
 *   OK on success (including if 0 voters are found)
 *   CONSTRAINT_VIOLATION if the new voter would violate a constraint
 *     (e.g. a voter exists with the same PII)
 */
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
               int64_t *out_id);

/* Modify the voter record whose id is voter_id
 *
 * Returns
 *   OK on success (including if 0 voters are found)
 *   CONSTRAINT_VIOLATION if the new voter would violate a constraint
 *     (e.g. a voter exists with the same PII)
 */
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
                         int64_t confidential);

/* Modify the voter record whose id is voter_id, such that the status is now
 * new_status.
 *
 * Returns
 *   OK on success (including if 0 voters are found)
 *   ERROR if some internal error occurs
 */
status_t
update_voter_status(bvrs_ctxt_t *ctxt,
                    int64_t voter_id,
                    enum regstatus new_status);

/* Search for voters by lastname, givenname, and birthdate _range_.
 *
 * Returns
 *   OK on success (including if 0 voters are found)
 *   ERROR if some internal error occurs
 */
status_t
query_voter_database(bvrs_ctxt_t *ctxt,
                     const char *lastname,
                     const char *givennames,
                     const char *resaddress,
                     const char *mailaddress,
                     time_t birthdate_lower,
                     time_t birthdate_upper,
                     struct voter_q **the_voters);

/* Create a new election official session.
 *
 * username/password are the login credentials of an official in the database.
 *
 * Returns
 *   OK if a new session is created
 *   NOT_FOUND if the username/password do not match the database record
 *   ERROR if some internal error occurs
 */
status_t
new_official_session(bvrs_ctxt_t *ctxt,
                     const char *username,
                     const char *password,
                     int64_t *session_id,
                     int64_t *token);

/* Explicitly end a session and remove it from the database.
 *
 * Returns
 *   OK on success
 *   ERROR if some internal error occurs
 */
status_t
end_official_session(bvrs_ctxt_t *ctxt,
                     int64_t the_session_id,
                     int64_t the_token);
#ifdef DEBUG
#define DBG(...) fprintf(stderr, __VA_ARGS__)
#else
#define DBG(...)
#endif

#define DBG_STR(_s) DBG(#_s "=%s", _s);

#endif //__BACKEND__
