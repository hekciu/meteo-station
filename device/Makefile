all: thing

thing: main.c
	gcc -Wall -pthread main.c -o thing -lpigpio -lrt

test: test.c
	gcc -Wall -pthread test.c -o test -lcurl
