#include <stdlib.h>
#include <stdio.h>
#include <regex.h>
#include <netinet/in.h>
#include <unistd.h>

#include "_utils.h"
#include "auth.h"

const int BUFFER_SIZE = 4096;

void * handle_client(void * arg) {
	int client_fd = *((int *)arg);
	char * buffer = (char *)malloc(BUFFER_SIZE * sizeof(char));

	ssize_t bytes_received = recv(client_fd, buffer, BUFFER_SIZE, 0);

	if (bytes_received > 0) {
        regex_t regex;
        regcomp(&regex, "^GET /([^ ]*) HTTP/1", REG_EXTENDED);
        regmatch_t matches[2];

        int reti = regexec(&regex, buffer, 2, matches, 0);

        if (reti == 0) {
            printf("got http request\n");
            char * authHeaderContent = NULL;
            int authHeaderLength = extract_header(buffer, "Authorization", &authHeaderContent);
            int authResult;
            if (authHeaderLength <= 0) {
                authResult = -1;
            } else {
                authResult = auth(authHeaderContent);
            }

            size_t responseSize;
			char * response = NULL;
            if (authResult != 0) {
                printf("auth failed\n");
                responseSize = build_response_unauthorized(&response);
            } else {
                printf("client authorized successfully\n"); 
                responseSize = build_response(&response);
            }

            send(client_fd, response, responseSize, 0);
            if (response != NULL) {
                free(response);
            }
            close(client_fd);
        }

        regfree(&regex);
	}
    free(buffer);
}
