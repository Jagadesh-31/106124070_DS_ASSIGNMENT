#ifndef PLATFORM_H
#define PLATFORM_H

#include <stdbool.h>
#include "post.h"

typedef struct Platform Platform;

Platform* createPlatform(void);
bool addPost(char* username, char* caption);
bool deletePost(int n);
Post* viewPost(int n);
Post* currPost(void);
Post* nextPost(void);
Post* previousPost(void);
bool addComment(char* username, char* content);
bool deleteComment(int n);
Comment* viewComments(void);
bool addReply(char* username, char* content, int n);
bool deleteReply(int n, int m);
void freePlatform(void);

#endif