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

    struct voter *voters;
    size_t n_voters;
    status_t lookupok = lookup_voter_information(ctxt, "lastname", "firstname", now, 0, &voters, &n_voters);
    assert(lookupok == OK);
    assert(n_voters == 1);
    assert(id == voters[0].id);

    db_voter_free(&voters[0]);

    status_t lookupok2 = lookup_voter_information(ctxt, "otherlastname", "firstname", now, 0, &voters, &n_voters);
    assert(lookupok2 == OK);
    assert(n_voters == 0);

    status_t closeok = close_db(ctxt);
    assert(closeok == OK);

    printf("Test Passed.");
}
