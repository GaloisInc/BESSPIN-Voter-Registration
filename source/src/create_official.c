#include <inttypes.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <stdbool.h>

#include <kcgi.h>
#include <kcgijson.h>
#include "backend.h"

void usage() {
    printf("usage: create_official <sqlite.db> <username> <password>\n");
}

int main(int argc, char **argv) {
    if(argc != 4) {
        usage();
        exit(EXIT_FAILURE);
    }

    char* username = argv[2];
    char* pass = argv[3];
    bvrs_ctxt_t *ctxt;

    if(strlen(pass) < 7) {
        printf("Password must be at-least 7 characters.\n");
        usage();
        exit(EXIT_FAILURE);
    }

    status_t openok = open_db(argv[1], &ctxt);
    if (openok != OK) {
        printf("Unable to open database %s\n", argv[1]);
        usage();
        return EXIT_FAILURE;
    }

    int64_t status  = db_electionofficial_insert(ctxt, username, pass);
    close_db(ctxt);
    if(status < 0) {
        printf("Error creating official.  code %ld\n", status);
        exit(EXIT_FAILURE);
    }
    exit(EXIT_SUCCESS);
}