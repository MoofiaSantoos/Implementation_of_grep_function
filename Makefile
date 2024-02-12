CC = gcc
CFLAGS = -Wall -Werror -Wextra -std=c11

all: grep

grep: clean
	$(CC) $(CFLAGS) grep.c -o grep

clean: 
	rm -rf grep

rebuild: all

clang:
	clang-format -i *.c *.h