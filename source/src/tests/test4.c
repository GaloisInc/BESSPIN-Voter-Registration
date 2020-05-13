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

    struct voterupdatesession *the_session;
    status_t sessionok = new_voter_session(ctxt,
                                           "lastname",
                                           "firstname",
                                           "addr",
                                           "addr2",
                                           now,
                                           &data[0],
                                           sizeof(data),
                                           0,
                                           &the_session);
    assert(sessionok == OK);

    status_t updateok = update_voter_information(ctxt,
                                                 the_session,
                                                 "lastname2",
                                                 "firstname2",
                                                 "addr",
                                                 "addr2",
                                                 "not whig",
                                                 now,
                                                 &data[0],
                                                 sizeof(data),
                                                 0);
    assert(updateok == OK);

    struct voter *voters;
    size_t n_voters;

    status_t lookupok = lookup_voter_information(ctxt, "lastname", "firstname", now, &data[0], sizeof(data), 0, &voters, &n_voters);
    assert(lookupok == OK);
    assert(n_voters == 0);

    status_t lookupok2 = lookup_voter_information(ctxt, "lastname2", "firstname2", now, &data[0], sizeof(data), 0, &voters, &n_voters);
    assert(lookupok2 == OK);
    assert(n_voters == 1);
    assert(voters[0].id == id);

    status_t endsessionok = end_voter_session(ctxt, the_session);
    assert(endsessionok == OK);

    status_t closeok = close_db(ctxt);
    assert(closeok == OK);

    printf("Test Passed.");
}
