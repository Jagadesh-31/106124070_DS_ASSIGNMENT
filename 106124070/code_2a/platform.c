#include "platform.h"
#include "comment.h"
#include "reply.h"
#include <stdlib.h>
#include <string.h>

typedef struct PostNode {
    Post* post;
    struct PostNode* next;
} PostNode;

// Single global platform - created once
Platform* globalPlatform = NULL;

struct Platform {
    PostNode* posts;        // newest post at front
    Post* lastViewedPost;
};

Platform* createPlatform(void) {
    if (globalPlatform) return globalPlatform;
    globalPlatform = malloc(sizeof(Platform));
    if (!globalPlatform) return NULL;
    globalPlatform->posts = NULL;
    globalPlatform->lastViewedPost = NULL;
    return globalPlatform;
}

bool addPost(char* username, char* caption) {
    Post* p = createPost(username, caption);
    if (!p) return false;
    PostNode* node = malloc(sizeof(PostNode));
    if (!node) {
        freePost(p);
        return false;
    }
    node->post = p;
    node->next = globalPlatform->posts;
    globalPlatform->posts = node;
    return true;
}

bool deletePost(int n) {
    if (n < 1 || !globalPlatform->posts) return false;
    PostNode* prev = NULL;
    PostNode* curr = globalPlatform->posts;
    int i;
    for (i = 1; i < n; ++i) {  // Traverse to nth node from head
        if (!curr->next) return false;
        prev = curr;
        curr = curr->next;
    }
    Post* toDelete = curr->post;
    if (prev) {
        prev->next = curr->next;  // Bypass the node to delete
    } else {
        globalPlatform->posts = curr->next;  // Update head if deleting first
    }
    free(curr);
    freePost(toDelete);
    if (globalPlatform->lastViewedPost == toDelete) {
        globalPlatform->lastViewedPost = globalPlatform->posts ? globalPlatform->posts->post : NULL;
    }
    return true;
}

Post* viewPost(int n) {
    if (n < 1 || !globalPlatform->posts) return NULL;
    PostNode* curr = globalPlatform->posts;
    int i;
    for (i = 1; i < n; ++i) {  // Skip to nth node
        if (!curr->next) return NULL;
        curr = curr->next;
    }
    globalPlatform->lastViewedPost = curr->post;  // Update last viewed
    return curr->post;
}

Post* currPost(void) {
    if (globalPlatform->lastViewedPost) return globalPlatform->lastViewedPost;
    return globalPlatform->posts ? globalPlatform->posts->post : NULL;  // Default to newest
}

Post* nextPost(void) {
    Post* lv = globalPlatform->lastViewedPost;
    if (!lv) return NULL;
    PostNode* node = globalPlatform->posts;
    while (node && node->post != lv) {  // Find node of current last viewed
        node = node->next;
    }
    if (!node) return NULL;
    PostNode* older = node->next;  // Next is older post (since newest first)
    if (older) {
        globalPlatform->lastViewedPost = older->post;
        return older->post;
    }
    return lv;  // Already at oldest, stay
}

Post* previousPost(void) {
    Post* lv = globalPlatform->lastViewedPost;
    if (!lv) return NULL;
    PostNode* prev = NULL;
    PostNode* node = globalPlatform->posts;
    while (node && node->post != lv) {  // Traverse to find current, tracking previous
        prev = node;
        node = node->next;
    }
    if (!node) return NULL;
    if (prev) {
        globalPlatform->lastViewedPost = prev->post;  // Previous is newer post
        return prev->post;
    }
    return lv;  // Already at newest, stay
}

bool addComment(char* username, char* content) {
    Post* p = globalPlatform->lastViewedPost;
    if (!p) return false;
    Comment* c = createComment(username, content);
    if (!c) return false;
    if (!p->comments) {
        p->comments = c;
    } else {
        Comment* tail = p->comments;
        while (tail->next) tail = tail->next;  // Find end to append (chronological order)
        tail->next = c;
    }
    return true;
}

bool deleteComment(int n) {
    Post* p = globalPlatform->lastViewedPost;
    if (!p) return false;
    int len = 0;
    Comment* c;
    for (c = p->comments; c; c = c->next) ++len;  // Count total comments
    if (n < 1 || n > len) return false;
    int pos = len - n + 1;  // Convert recent index to position from head
    Comment* prev = NULL;
    c = p->comments;
    int i;
    for (i = 1; i < pos; ++i) {
        prev = c;
        c = c->next;
    }
    if (prev) {
        prev->next = c->next;
    } else {
        p->comments = c->next;
    }
    freeComment(c);
    return true;
}

Comment* viewComments(void) {
    Post* p = globalPlatform->lastViewedPost;
    return p ? p->comments : NULL;
}

bool addReply(char* username, char* content, int n) {
    Post* p = globalPlatform->lastViewedPost;
    if (!p) return false;
    int len = 0;
    Comment* c;
    for (c = p->comments; c; c = c->next) ++len;
    if (n < 1 || n > len) return false;
    int pos = len - n + 1;  // Recent to absolute position
    Comment* target = p->comments;
    int i;
    for (i = 1; i < pos; ++i) {
        target = target->next;
    }
    Reply* r = createReply(username, content);
    if (!r) return false;
    if (!target->replies) {
        target->replies = r;
    } else {
        Reply* tail = target->replies;
        while (tail->next) tail = tail->next;
        tail->next = r;
    }
    return true;
}

bool deleteReply(int n, int m) {
    Post* p = globalPlatform->lastViewedPost;
    if (!p) return false;
    int lenC = 0;
    Comment* c;
    for (c = p->comments; c; c = c->next) ++lenC;
    if (n < 1 || n > lenC) return false;
    int posC = lenC - n + 1;  // Comment position from head
    Comment* targetC = p->comments;
    int i;
    for (i = 1; i < posC; ++i) {
        targetC = targetC->next;
    }
    int lenR = 0;
    Reply* r;
    for (r = targetC->replies; r; r = r->next) ++lenR;  // Count replies on target comment
    if (m < 1 || m > lenR) return false;
    int posR = lenR - m + 1;  // Reply position from head
    Reply* prevR = NULL;
    r = targetC->replies;
    int j;
    for (j = 1; j < posR; ++j) {
        prevR = r;
        r = r->next;
    }
    if (prevR) {
        prevR->next = r->next;
    } else {
        targetC->replies = r->next;
    }
    freeReply(r);
    return true;
}

void freePlatform(void) {
    if (!globalPlatform) return;
    PostNode* node = globalPlatform->posts;
    while (node) {
        PostNode* next = node->next;
        freePost(node->post);
        free(node);
        node = next;
    }
    free(globalPlatform);
    globalPlatform = NULL;
}