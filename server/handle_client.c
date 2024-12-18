#include <stdlib.h>
#include <stdio.h>
#include <regex.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>

#include "_utils.h"
#include "_database.h"
#include "auth.h"
#include "_parse.h"

#define BUFFER_SIZE 4096

size_t _handle_get_request(char * endpointStr, int authResult, char ** response) {
    size_t responseSize;
    const char * PMS_5003_ENDPOINT = "/measurements/PMS5003";

    if (strncmp(endpointStr, PMS_5003_ENDPOINT, strlen(PMS_5003_ENDPOINT)) == 0) {
        char * timestampFrom = NULL;
        char * timestampTo = NULL;
        if (extract_query_param(endpointStr, "timestampFrom", &timestampFrom) != 0) {
            fprintf(stderr, "Missing timestampFrom param\n"); 
            timestampFrom = malloc(0);
            responseSize = build_response_bad_request(response, "missing timestampfrom query param"); 
        } else if (extract_query_param(endpointStr, "timestampTo", &timestampTo) != 0) {
            fprintf(stderr, "Missing timestampTo param\n"); 
            timestampTo = malloc(0);
            responseSize = build_response_bad_request(response, "missing timestampTo query param"); 
        } else {
            char * fromError = malloc(strlen(timestampFrom) + 1);
            char * toError = malloc(strlen(timestampTo) + 1);
            int64_t from = strtoll(timestampFrom, &fromError, 10); // fromError and toError should not be freed
            int64_t to = strtoll(timestampTo, &toError, 10);
            if (strlen(fromError) != 0 || strlen(toError) != 0) {
                char * text = "timestampTo and timestampFrom need to be valid integers";
                fprintf(stderr, text);
                responseSize = build_response_bad_request(response, text); 
            } else {
                char * content = NULL;
                if (get_PMS5003_measurements((uint64_t)from, (uint64_t)to, &content) == 0) {
                    responseSize = build_response(response, content);
                    free(content);
                } else {
                    responseSize = build_response_internal_server_error(response, "error with database connection\n");
                }
            }
        }

        free(timestampTo);
        free(timestampFrom);
    } else {
        fprintf(stderr, "not found\n");
        responseSize = build_response_not_found(response);
    }
    return responseSize;
};

size_t _handle_post_request(char * endpointStr, int authResult, char * requestBody, char ** response) {
    size_t responseSize;

    if (strcmp(endpointStr, "/measurements/PMS5003") == 0) {
        if (authResult != 0) {
            fprintf(stderr, "auth failed\n");
            responseSize = build_response_unauthorized(response);
        } else {
            printf("client authorized successfully\n"); 
            uint64_t device_timestamp;
            char * device_name;
            uint16_t pm10_standard;
            uint16_t pm25_standard;
            uint16_t pm100_standard;
            if (parseInsertPMS5003Body(requestBody, &device_timestamp, &device_name, &pm10_standard, &pm25_standard, &pm100_standard) == 0) {
                int insertStatus = insert_PMS5003_measurement(device_timestamp, device_name, pm10_standard, pm25_standard, pm100_standard);
                responseSize = insertStatus == 0 ? build_response(response, "OK") : build_response_internal_server_error(response, "database error\n");

                free(device_name);
            } else {
                responseSize = build_response_bad_request(response, "could not parse PMS5003 data body\n"); 
            }
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
            int authResult = extract_header(buffer, "Authorization", &authHeaderContent) == 0 ? auth(authHeaderContent) : -1;

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
            if (extract_request_body(buffer, &requestBody) == 0) {
                printf("With body:\n%s\n", requestBody);
            }

            char * authHeaderContent = NULL;
            int authResult = extract_header(buffer, "Authorization", &authHeaderContent) == 0 ? auth(authHeaderContent) : -1;

            // TODO: add check for Content-Type header
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
};
