all: server

server: main.c handle_client.c build_response.c extract_header.c
	gcc -Wall -o server -lpthread main.c handle_client.c build_response.c extract_header.c
