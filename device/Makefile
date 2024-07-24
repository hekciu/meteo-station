current_dir := $(shell pwd)

all: thing

thing: main.c
	gcc -Wall -pthread main.c -o thing -lpigpio -lrt -lcurl

test: test.c httpRequests.h
	gcc -Wall -pthread test.c -o test -lcurl
