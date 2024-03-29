#include <backend.h>
#include <assert.h>
#include <stdio.h>

int
main(int argc, char **argv)
{
    bvrs_ctxt_t *ctxt;

    status_t openok = open_db(argv[1], &ctxt);
    assert(openok == OK);

    status_t closeok = close_db(ctxt);
    assert(closeok == OK);

    printf("[0] Test Passed.\n");
}
