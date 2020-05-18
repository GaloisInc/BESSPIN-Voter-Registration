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

    int64_t sid, tok;
    status_t sess_ok = new_official_session(ctxt, "love2vote", "love2vote", &sid, &tok);
    assert(sess_ok == OK);

    sess_ok = new_official_session(ctxt, "love2vote", "hate2vote", &sid, &tok);
    assert(sess_ok != OK);

    printf("Test Passed.");
}
