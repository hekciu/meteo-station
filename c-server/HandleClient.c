#include <stdlib.h>
#include <stdio.h>
#include <netinet/in.h>

#include "constants.h"

void * HandleClient(void * arg) {
	int client_fd = *((int *)arg);
	
	char * buffer = (char *)malloc(BUFFER_SIZE * sizeof(char));

	ssize_t bytes_received = recv(client_fd, buffer, BUFFER_SIZE, 0);
    printf("bytes_received: %ld\n", bytes_received);
    free(buffer);
}
