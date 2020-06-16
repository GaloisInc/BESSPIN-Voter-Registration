#include <backend.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "db.h"

int
main(int argc, char **argv)
{
    char *data = "blob1";
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
                                    data,
                                    strlen(data),
                                    0,
                                    REGSTATUS_ACTIVE,
                                    &id);
    assert(regok == OK);

    int64_t sid, tok;
    struct voter *voter;
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
                                           data,
                                           strlen(data),
                                           0,
                                           &voter,
                                           &sid,
                                           &tok);
    assert(sessionok == OK);

    status_t endsessionok = end_voter_session(ctxt, id, tok);
    assert(endsessionok == OK);

    status_t closeok = close_db(ctxt);
    assert(closeok == OK);

    printf("[3] Test Passed.\n");
}
