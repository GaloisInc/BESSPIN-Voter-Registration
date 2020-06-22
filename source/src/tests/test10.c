#include <assert.h>
#include <string.h>
#include <stdio.h>

#include "backend.h"
#include "db.h"

void dummy_page(struct kreq *r)
{
    printf("dummy called\n");
}

int
main(int argc, char **argv)
{
    bvrs_ctxt_t *ctxt;

    status_t openok = open_db(argv[1], &ctxt);
    assert(openok == OK);

    struct kreq r;
    r.arg = ctxt;
    struct kpair token;
    struct kpair sid;
    struct kpair *cookies = calloc(2, sizeof(struct kpair*));
    sid.parsed.i = 0;
    token.parsed.s = "ABCDEFGH12345678ABCDEFGH12345678";
    r.cookiemap = &cookies;
    r.cookiemap[VALID_ELECTIONOFFICIALSESSION_ID] = &sid;
    r.cookiemap[VALID_ELECTIONOFFICIALSESSION_TOKEN] = &token;
    status_t status = require_official(dummy_page, &r);
    assert(status == OK);

    printf("[10] Test Passed - Require Official\n");
    free(cookies);
}
