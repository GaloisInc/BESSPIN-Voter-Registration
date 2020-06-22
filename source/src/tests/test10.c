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

    r.cookiemap = calloc(VALID__MAX, sizeof(struct kpair*));
    r.cookiemap[VALID_ELECTIONOFFICIALSESSION_ID] = malloc(sizeof(struct kpair));
    r.cookiemap[VALID_ELECTIONOFFICIALSESSION_ID]->parsed.i = 1;
    r.cookiemap[VALID_ELECTIONOFFICIALSESSION_TOKEN] = malloc(sizeof(struct kpair));
    r.cookiemap[VALID_ELECTIONOFFICIALSESSION_TOKEN]->parsed.s = "ABCDEFGH12345678ABCDEFGH12345678";
    //printf("cookiemap before: %s\n", r.cookiemap[VALID_ELECTIONOFFICIALSESSION_TOKEN]->parsed.s);
    status_t status = require_official(dummy_page, &r);
    assert(status == OK);
    // Ensure that the guarded page is called
    assert(r.port == 345);

    printf("[10] Test Passed - Require Official\n");
}
