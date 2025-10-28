#include "comment.h"
#include "reply.h"
#include <stdlib.h>
#include <string.h>

Comment* createComment(char* username, char* content) {
    Comment* c = malloc(sizeof(Comment));
    if (!c) return NULL;
    c->username = malloc(strlen(username) + 1);
    strcpy(c->username, username);
    c->content = malloc(strlen(content) + 1);
    strcpy(c->content, content);
    c->replies = NULL;
    c->next = NULL;
    return c;
}

void freeComment(Comment* c) {
    if (!c) return;
    Reply* r = c->replies;
    while (r) {
        Reply* next = r->next;
        freeReply(r);
        r = next;
    }
    free(c->username);
    free(c->content);
    free(c);
}