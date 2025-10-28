#include <stdio.h>
#include <string.h>
#include "platform.h"
#include "post.h"
#include "comment.h"
#include "reply.h"

#define MAX_INPUT 256

void printPost(Post* post) {
    if (post) {
        printf("%s %s\n", post->username, post->caption);
    }
}

void printAllComments(Comment* commentHead) {
    Comment* current = commentHead;
    while (current) {
        printf("%s %s\n", current->username, current->content);
        Reply* replyPtr = current->replies;
        while (replyPtr) {
            printf("%s %s\n", replyPtr->username, replyPtr->content);
            replyPtr = replyPtr->next;
        }
        current = current->next;
    }
}

int main() {
    char command[MAX_INPUT];
    while (scanf("%s", command) == 1) {
        if (strcmp(command, "create_platform") == 0) {
            createPlatform();
        } else if (strcmp(command, "add_post") == 0) {
            char username[MAX_INPUT], caption[MAX_INPUT];
            scanf("%s %s", username, caption);
            addPost(username, caption);
        } else if (strcmp(command, "delete_post") == 0) {
            int n;
            scanf("%d", &n);
            if (!deletePost(n)) {
                printf("Post does not exist.\n");
            }
        } else if (strcmp(command, "view_post") == 0) {
            int n;
            scanf("%d", &n);
            Post* post = viewPost(n);
            if (post) {
                printPost(post);
            } else {
                printf("Post does not exist.\n");
            }
        } else if (strcmp(command, "current_post") == 0) {
            Post* post = currPost();
            if (post) {
                printPost(post);
            } else {
                printf("No current post.\n");
            }
        } else if (strcmp(command, "next_post") == 0) {
            Post* post = nextPost();
            if (post) {
                printPost(post);
            } else {
                printf("Next post does not exist.\n");
            }
        } else if (strcmp(command, "previous_post") == 0) {
            Post* post = previousPost();
            if (post) {
                printPost(post);
            } else {
                printf("Previous post does not exist.\n");
            }
        } else if (strcmp(command, "add_comment") == 0) {
            char username[MAX_INPUT], content[MAX_INPUT];
            scanf("%s %s", username, content);
            if (!addComment(username, content)) {
                printf("No post selected for comment.\n");
            }
        } else if (strcmp(command, "delete_comment") == 0) {
            int n;
            scanf("%d", &n);
            if (!deleteComment(n)) {
                printf("Comment does not exist.\n");
            }
        } else if (strcmp(command, "view_comments") == 0 || strcmp(command, "view_all_comments") == 0) {
            Comment* comments = viewComments();
            if (comments) {
                printAllComments(comments);
            }
        } else if (strcmp(command, "add_reply") == 0) {
            char username[MAX_INPUT], content[MAX_INPUT];
            int n;
            scanf("%s %s %d", username, content, &n);
            if (!addReply(username, content, n)) {
                printf("No comment selected for reply.\n");
            }
        } else if (strcmp(command, "delete_reply") == 0) {
            int n, m;
            scanf("%d %d", &n, &m);
            if (!deleteReply(n, m)) {
                printf("Reply does not exist.\n");
            }
        } else if (strcmp(command, "exit") == 0 || strcmp(command, "quit") == 0) {
            break;
        }
    }
    freePlatform();
    return 0;
}