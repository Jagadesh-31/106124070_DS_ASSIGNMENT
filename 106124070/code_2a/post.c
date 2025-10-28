#include "post.h"
#include "comment.h"
#include <stdlib.h>
#include <string.h>

Post* createPost(char* username, char* caption) {
    Post* p = malloc(sizeof(Post));
    if (!p) return NULL;
    p->username = malloc(strlen(username) + 1);
    strcpy(p->username, username);
    p->caption = malloc(strlen(caption) + 1);
    strcpy(p->caption, caption);
    p->comments = NULL;
    return p;
}

void freePost(Post* p) {
    if (!p) return;
    Comment* c = p->comments;
    while (c) {
        Comment* next = c->next;
        freeComment(c);
        c = next;
    }
    free(p->username);
    free(p->caption);
    free(p);
}