#ifndef COMMENT_H
#define COMMENT_H

typedef struct Reply Reply;
typedef struct Comment Comment;

struct Comment {
    char* username;
    char* content;
    Reply* replies;
    Comment* next;
};

Comment* createComment(char* username, char* content);
void freeComment(Comment* c);

#endif