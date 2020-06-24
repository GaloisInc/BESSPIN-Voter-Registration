#include <backend.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "db.h"

int
main(int argc, char **argv)
{
    bvrs_ctxt_t *ctxt;

    status_t openok = open_db(argv[1], &ctxt);
    assert(openok == OK);

    int64_t sid;
    char tok[TOKEN_SIZE] = "";
    status_t sess_ok = new_official_session(ctxt, "love2vote", "love2vote", &sid, tok);
    assert(sess_ok == OK);
    assert(sid > 0);
    // printf("token[%lu]: %s\n", strlen(tok), tok);
    assert(strlen(tok) == TOKEN_SIZE - 1);
    
    strcpy(tok, "");
    sess_ok = new_official_session(ctxt, "love2vote", "hate2vote", &sid, tok);
    assert(sess_ok != OK);
    assert(strcmp(tok, "") == 0);

    printf("[7] Test Passed - New Official Session.\n");
    close_db(ctxt);
}
