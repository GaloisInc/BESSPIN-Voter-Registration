#include <backend.h>
#include <assert.h>
#include <stdio.h>

#include "db.h"

int
main(int argc, char **argv)
{
    char data[16];
    time_t now;
    bvrs_ctxt_t *ctxt;

    status_t openok = open_db(argv[1], &ctxt);
    assert(openok == OK);

    /*  Insert a voter into the DB and then query */
    time(&now);
    int64_t id;
    status_t regok = register_voter(ctxt,
                                    "lastname",
                                    "firstname",
                                    "1234 addr",
                                    "1234 addr2",
                                    "00000",
                                    "ZZ",
                                    "1234 addr",
                                    "1234 addr2",
                                    "00000",
                                    "ZZ",
                                    "whig",
                                    now,
                                    &data[0],
                                    sizeof(data),
                                    0,
                                    REGSTATUS_PENDINGREVIEW,
                                    &id);
    assert(regok == OK);

    struct voter *voter;
    int64_t sid, tok;
    status_t sessionok = new_voter_session(ctxt,
                                           "lastname",
                                           "firstname",
                                           "1234 addr",
                                           "1234 addr2",
                                           "00000",
                                           "ZZ",
                                           "1234 addr",
                                           "1234 addr2",
                                           "00000",
                                           "ZZ",
                                           now,
                                           &data[0],
                                           sizeof(data),
                                           0,
                                           &voter, &sid, &tok);
    assert(sessionok == OK);

    status_t updateok = update_voter_information(ctxt,
                                                 voter->id,
                                                 "lastname2",
                                                 "firstname2",
                                                 "1234 addr",
                                                 "1234 addr2",
                                                 "00000",
                                                 "ZZ",
                                                 "1234 addr",
                                                 "1234 addr2",
                                                 "00000",
                                                 "ZZ",
                                                 "not_whig",
                                                 now,
                                                 &data[0],
                                                 sizeof(data),
                                                 REGSTATUS_ACTIVE,
                                                 0);
    assert(updateok == OK);

    struct voter_q *voters;
    status_t lookupok = lookup_voter_information(ctxt, "lastname", "firstname", now, 0, &voters);
    assert(lookupok == OK);
    assert(NULL == TAILQ_FIRST(voters));
    db_voter_freeq(voters);

    status_t lookupok2 = lookup_voter_information(ctxt, "lastname2", "firstname2", now, 0, &voters);
    assert(lookupok2 == OK);
    voter = TAILQ_FIRST(voters);
    assert(voter != NULL);
    assert(voter->id == id);
    TAILQ_REMOVE(voters, voter, _entries);
    assert(NULL == TAILQ_FIRST(voters));

    status_t endsessionok = end_voter_session(ctxt, sid, tok);
    assert(endsessionok == OK);

    status_t closeok = close_db(ctxt);
    assert(closeok == OK);

    printf("[4] Test Passed.\n");
}
