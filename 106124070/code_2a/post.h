#ifndef POST_H
#define POST_H

typedef struct Comment Comment;

typedef struct Post Post;

struct Post {
    char* username;
    char* caption;
    Comment* comments;
};

Post* createPost(char* username, char* caption);
void freePost(Post* p);

#endif