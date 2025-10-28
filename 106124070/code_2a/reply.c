#include "reply.h"
#include <stdlib.h>
#include <string.h>

Reply* createReply(char* username, char* content) {
    Reply* r = malloc(sizeof(Reply));
    if (!r) return NULL;
    r->username = malloc(strlen(username) + 1);
    strcpy(r->username, username);
    r->content = malloc(strlen(content) + 1);
    strcpy(r->content, content);
    r->next = NULL;
    return r;
}

void freeReply(Reply* r) {
    if (!r) return;
    free(r->username);
    free(r->content);
    free(r);
}