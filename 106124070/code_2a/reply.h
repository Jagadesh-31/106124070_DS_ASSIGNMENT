#ifndef REPLY_H
#define REPLY_H

typedef struct Reply Reply;

struct Reply {
    char* username;
    char* content;
    Reply* next;
};

Reply* createReply(char* username, char* content);
void freeReply(Reply* r);

#endif