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
                                    "addr",
                                    "addr2",
                                    "00000",
                                    "ZZ",
                                    "addr",
                                    "addr2",
                                    "00000",
                                    "ZZ",
                                    "whig",
                                    now,
                                    &data[0],
                                    sizeof(data),
                                    0,
                                    &id);
    assert(regok == OK);

    struct voter_q *voters;
    struct voter *voter;
    status_t lookupok = lookup_voter_information(ctxt, "lastname", "firstname", now, 0, &voters);
    assert(lookupok == OK);

    voter = TAILQ_FIRST(voters);
    assert(voter != NULL);
    assert(voter->id == id);
    TAILQ_REMOVE(voters, voter, _entries);
    assert(NULL == TAILQ_FIRST(voters));
    db_voter_free(voter);
    db_voter_freeq(voters);

    status_t lookupok2 = lookup_voter_information(ctxt, "otherlastname", "firstname", now, 0, &voters);
    assert(lookupok2 == OK);
    assert(NULL == TAILQ_FIRST(voters));

    status_t closeok = close_db(ctxt);
    assert(closeok == OK);

    printf("[2] Test Passed.\n");
}
