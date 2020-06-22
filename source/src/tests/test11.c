#include <assert.h>
#include <string.h>
#include <stdio.h>

#include "backend.h"
#include "db.h"

enum	kstate {
	KSTATE_HEAD = 0,
	KSTATE_BODY
};

/*
 * Interior data.
 * This is used for managing HTTP compression.
 */
struct	kdata {
	int		 debugging; /* debugging flags */
	int		 fcgi; /* file descriptor or -1 */
	int		 control; /* control socket or -1 */
	char		*linebuf; /* output line buffer */
	size_t		 linebufpos; /* output line buffer */
	size_t		 linebufsz;
	uint64_t	 bytes; /* total bytes written */
	uint16_t	 requestId; /* current requestId or 0 */
	enum kstate	 state;
};



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
    r.cookiemap[VALID_ELECTIONOFFICIALSESSION_TOKEN]->parsed.s = "11111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111";

    status_t status = require_official(dummy_page, &r);
    assert(status == OK);
    // Ensure that the guarded page is called
    assert(r.port == 345);

    printf("[11] Test Passed - HACK Official\n");
}
