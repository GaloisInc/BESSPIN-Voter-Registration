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

    struct voterupdatesession *sess = db_voterupdatesession_get_updatecreds(ctxt, 0, "ASDFG");
    assert(sess);

    flush_old_sessions(ctxt, 5, 5); // 5 second timeout
    sess = db_voterupdatesession_get_updatecreds(ctxt, 0, "ASDFG");
    assert(NULL == sess);

    printf("[6] Test Passed.\n");
}
