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
                                    "whig",
                                    now,
                                    &data[0],
                                    sizeof(data),
                                    0,
                                    &id);
    assert(regok == OK);


    status_t updateok = update_voter_status(ctxt, id, REGSTATUS_INACTIVE);
    assert(updateok == OK);


    struct voter_q *voters;
    struct voter *voter;
    status_t lookupok = lookup_voter_information(ctxt, "lastname", "firstname", now, 0, &voters);
    assert(lookupok == OK);
    voter = TAILQ_FIRST(voters);
    assert(voter->status == REGSTATUS_INACTIVE);

    status_t closeok = close_db(ctxt);
    assert(closeok == OK);

    printf("[5] Test Passed.\n");
}
