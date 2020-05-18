#include <backend.h>
#include <assert.h>
#include <stdio.h>

#include "db.h"

int
main(int argc, char **argv)
{
    char data[16] = "blob";
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

    status_t closeok = close_db(ctxt);
    assert(closeok == OK);

    printf("Test Passed.");
}
