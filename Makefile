CC = gcc
CFLAGS = -Wall -Werror -Wextra -std=c11

all:
	$(CC) $(CFLAGS) grep.c -o mygrep

clean: 
	rm -rf mygrep

rebuild: all

clang:
	clang-format -i *.c *.h