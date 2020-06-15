#include <backend.h>
#include <assert.h>
#include <stdio.h>

#include "db.h"

int
main(int argc, char **argv)
{
    bvrs_ctxt_t *ctxt;

    status_t openok = open_db(argv[1], &ctxt);
    assert(openok == OK);

    struct voter_q *voters;
    status_t queryok = query_voter_database(ctxt, "%%", "%%", "%%", "%%", -600000000, time(NULL), &voters);
    assert(queryok == OK);

    int num = 0;
    struct voter *voter;
    TAILQ_FOREACH(voter, voters, _entries) {
        num++;
    }
    assert(num == 2);

    printf("[8] Test Passed.\n");
}
