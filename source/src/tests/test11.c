#include <assert.h>
#include <string.h>
#include <stdio.h>

#include "backend.h"
#include "db.h"

void dummy_page(struct kreq *r)
{
    r->port = 345;
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
    memset(&token, 0, sizeof(struct kpair));
    memset(&sid, 0, sizeof(struct kpair));

    sid.parsed.i = 1;
    r.cookiemap = calloc(VALID__MAX, sizeof(struct kpair*));
    token.parsed.s = "111111111111111111111111111111111111111111111111111111111111111111111111111111111";
    r.cookiemap[VALID_ELECTIONOFFICIALSESSION_ID] = &sid;
    r.cookiemap[VALID_ELECTIONOFFICIALSESSION_TOKEN] = &token;

    status_t status = require_official(dummy_page, &r);
    assert(status == OK);
    // Ensure that the guarded page is called
    assert(r.port == 345);

    printf("[11] Test Passed - HACK Official\n");
}
