#include <stdlib.h>
#include <stdio.h>
#include <regex.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>

#include "_utils.h"
#include "auth.h"

const int BUFFER_SIZE = 4096;

size_t _handle_get_request(char * endpointStr, int authResult, char ** response) {
    size_t responseSize;

    if (strcmp(endpointStr, "/measurements") == 0) {
        printf("sending response\n");
        responseSize = build_response(response);
    } else {
        printf("not found\n");
        responseSize = build_response_not_found(response);
    }

    return responseSize;
};

size_t _handle_post_request(char * endpointStr, int authResult, char * requestBody, char ** response) {
    size_t responseSize;

    if (strcmp(endpointStr, "/data") == 0) {
        if (authResult != 0) {
            printf("auth failed\n");
            responseSize = build_response_unauthorized(response);
        } else {
            printf("client authorized successfully\n"); 
            responseSize = build_response(response);
        }
    } else {
        printf("not found\n");
        responseSize = build_response_not_found(response);
    }

    return responseSize;
};

void * handle_client(void * arg) {
	int client_fd = *((int *)arg);
	char * buffer = (char *)malloc(BUFFER_SIZE * sizeof(char));

	ssize_t bytes_received = recv(client_fd, buffer, BUFFER_SIZE, 0);

	if (bytes_received > 0) {
        regex_t get_regex, post_regex;

        regcomp(&get_regex, "^GET (/[^ ]*) HTTP/1", REG_EXTENDED);
        regcomp(&post_regex, "^POST (/[^ ]*) HTTP/1", REG_EXTENDED);

        regmatch_t matches[2];

        int get_reti = regexec(&get_regex, buffer, 2, matches, 0);
        int post_reti = regexec(&post_regex, buffer, 2, matches, 0);

	    char * response = NULL;
   
        if (get_reti == 0) {
            int endpointStrSize = matches[1].rm_eo - matches[1].rm_so;
            char * endpointStr = malloc(endpointStrSize + 1);
            memcpy(endpointStr, buffer + matches[1].rm_so, endpointStrSize);            
            *(endpointStr + endpointStrSize) = '\0';
            
            printf("HTTP GET %s\n", endpointStr);
            char * authHeaderContent = NULL;
            int authHeaderLength = extract_header(buffer, "Authorization", &authHeaderContent);
            int authResult = authHeaderLength <= 0 ? -1 : auth(authHeaderContent);

            size_t responseSize = _handle_get_request(endpointStr, authResult, &response); 

            free(endpointStr);
            free(authHeaderContent);

            send(client_fd, response, responseSize, 0);
        } else if (post_reti == 0) {
            int endpointStrSize = matches[1].rm_eo - matches[1].rm_so;
            char * endpointStr = malloc(endpointStrSize + 1);
            memcpy(endpointStr, buffer + matches[1].rm_so, endpointStrSize);            
            *(endpointStr + endpointStrSize) = '\0';

            printf("HTTP POST %s\n", endpointStr);
            char * requestBody = NULL;
            size_t requestBodySize = extract_request_body(buffer, &requestBody);
            if (requestBodySize > 0) {
                printf("With body:\n%s\n", requestBody);
            }

            char * authHeaderContent = NULL;
            int authHeaderLength = extract_header(buffer, "Authorization", &authHeaderContent);
            int authResult = authHeaderLength <= 0 ? -1 : auth(authHeaderContent);

            size_t responseSize = _handle_post_request(endpointStr, authResult, requestBody, &response); 

            free(endpointStr);
            free(requestBody);
            free(authHeaderContent);

            send(client_fd, response, responseSize, 0);
        } else {
            char * UNSUPPORTED_PROTOCOL_MESSAGE = "Got unsupported protocol or method, only HTTP/1 or HTTP/1.1 POST and GET are available\r\n";
            send(client_fd, UNSUPPORTED_PROTOCOL_MESSAGE, strlen(UNSUPPORTED_PROTOCOL_MESSAGE), 0); 
        }

        if (response != NULL) {
            free(response);
        }
        close(client_fd);

        regfree(&get_regex);
        regfree(&post_regex);
	}
    free(buffer);
}
