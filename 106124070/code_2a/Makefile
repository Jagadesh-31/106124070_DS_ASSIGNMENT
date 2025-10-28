CC = gcc
CFLAGS = -Wall -std=c99 -g

all: main

main: main.c platform.c post.c comment.c reply.c
	$(CC) $(CFLAGS) -o main main.c platform.c post.c comment.c reply.c

clean:
	rm -f main

.PHONY: all clean