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

    int64_t sid, tok;
    struct voter *voter;
    status_t sessionok = new_voter_session(ctxt,
                                           "lastname",
                                           "firstname",
                                           "addr",
                                           "addr2",
                                           now,
                                           &data[0],
                                           sizeof(data),
                                           0,
                                           &voter,
                                           &sid,
                                           &tok);
    assert(sessionok == OK);

    status_t endsessionok = end_voter_session(ctxt, id, tok);
    assert(endsessionok == OK);

    status_t closeok = close_db(ctxt);
    assert(closeok == OK);

    printf("Test Passed.");
}
