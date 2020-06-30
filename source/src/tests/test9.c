#include <backend.h>
#include <assert.h>
#include <string.h>
#include <stdio.h>

#include "db.h"

int
main(int argc, char **argv)
{
    bvrs_ctxt_t *ctxt;

    status_t openok = open_db(argv[1], &ctxt);
    assert(openok == OK);

    struct electionofficialsession *sess;
    sess = db_electionofficialsession_get_officialbyid(ctxt, 0);
    assert(strcmp(sess->token, "ABCDEFGH12345678ABCDEFGH12345678") == 0);
    db_electionofficialsession_free(sess);

    printf("[9] Test Passed - DB Get Election Official Session By ID\n");
    close_db(ctxt);
}
