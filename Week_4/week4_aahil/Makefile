CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -Wpedantic -Werror -g

.PHONY: all test clean
all: test_bankers

test_bankers: bankers.c tests/test_bankers.c bankers.h
	$(CC) $(CFLAGS) bankers.c tests/test_bankers.c -o test_bankers

test: test_bankers
	./test_bankers

clean:
	rm -f test_bankers
