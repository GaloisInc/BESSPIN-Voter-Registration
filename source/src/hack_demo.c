#include "stdio.h"
#include "string.h"

#define TOKEN_SIZE 33

int main(int argc, char** argv) {
    char blob[100] = "";
    char cookie_token[TOKEN_SIZE] = "";
    char verify_token[TOKEN_SIZE] = "";
    strcpy(cookie_token, "ASDF");
    strcpy(verify_token, argv[1]);
    printf("cookie: %s\n", cookie_token);
    printf("verify: %s\n", verify_token);
    if(strncmp(cookie_token, verify_token, 32) == 0) {
        printf("LOGIN SUCCESS\n");
    } else {
        printf("FAIL\n");
    }

    return 0;
}