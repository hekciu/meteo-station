#include <stdlib.h>
#include <stdio.h>
#include <regex.h>
#include <netinet/in.h>
#include <unistd.h>

#include "constants.h"
#include "build_response.h"
#include "extract_header.h"

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
			char * response = NULL;
            printf("got http request\n");
            char * authHeaderContent = NULL;
            size_t authHeaderSize = extract_header(buffer, "Authorization", &authHeaderContent);
            if (authHeaderSize < 0) {
                printf("auth failed, TODO: handle this\n");
            };
            size_t responseSize = build_response(&response);
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
