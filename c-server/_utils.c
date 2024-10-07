#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <regex.h>


size_t build_response_not_found(char ** response) {
    char * responseStart = "HTTP/1.1 404 Not Found \r\n";
    char * headers =
        "Content-Type: text/html; charset=utf-8\r\n"
        "Accept-Ranges: bytes\r\n"
        "Connection: keep-alive\r\n\r\n";
    char * bodyContent = "Not Found";
    size_t length = strlen(responseStart) + strlen(headers) + strlen(bodyContent) + 1;
    *response = malloc(length);
    int lengthWithoutNull = snprintf(*response, length, "%s%s%s", responseStart, headers, bodyContent);

    return (size_t)lengthWithoutNull;    
}


size_t build_response_unauthorized(char ** response) {
    char * responseStart = "HTTP/1.1 403 Forbidden \r\n";
    char * headers =
        "Content-Type: text/html; charset=utf-8\r\n"
        "Accept-Ranges: bytes\r\n"
        "Connection: keep-alive\r\n\r\n";
    char * bodyContent = "Unauthorized";
    size_t length = strlen(responseStart) + strlen(headers) + strlen(bodyContent) + 1;
    *response = malloc(length);
    int lengthWithoutNull = snprintf(*response, length, "%s%s%s", responseStart, headers, bodyContent);

    return (size_t)lengthWithoutNull;    
}


size_t build_response(char ** response) {
    char * responseStart = "HTTP/1.1 200 OK \r\n";
    char * headers =
        "Content-Type: text/html; charset=utf-8\r\n"
        "Accept-Ranges: bytes\r\n"
        "Connection: keep-alive\r\n\r\n";
    char * stringOrSomething = "Hello from GNU/Linux C socket program";
    size_t length = strlen(responseStart) + strlen(headers) + strlen(stringOrSomething) + 1;
    *response = malloc(length);
    int lengthWithoutNull = snprintf(*response, length, "%s%s%s", responseStart, headers, stringOrSomething);
    return (size_t)lengthWithoutNull;    
}


int extract_header(char * reqContent, char * headerName, char ** output) {
    if(*output != NULL) {
        free(*output);
        *output = NULL;
    } 

    const int BUFFER_SIZE = 512;

    regex_t regex;
    char * headerRegex = malloc(BUFFER_SIZE * sizeof(char));
    sprintf(headerRegex, "%s: .*", headerName);
    regcomp(&regex, headerRegex, REG_NEWLINE);
    regmatch_t matches[1];

    int reti = regexec(&regex, reqContent, 1, matches, 0);

    if (reti != 0) {
        fprintf(stderr, "Did not find requested header %s\n", headerName);
        return -1;
    }

    size_t headerNameSize = strlen(headerName);
    int headerContentStart = matches[0].rm_so + headerNameSize + 2;
    int headerContentEnd = matches[0].rm_eo;


    int headerContentSize = headerContentEnd - headerContentStart;
    *output = malloc(headerContentSize + 1);
    snprintf(*output, headerContentSize, reqContent + headerContentStart);
    
    return headerContentSize;
}

int extract_request_body(char * reqContent, char ** output) {
    if(*output != NULL) {
        free(*output);
        *output = NULL;
    } 

    regex_t regex;
    // TODO: this parser below, reference: https://codereview.stackexchange.com/questions/188384/http-request-parser-in-c
    char * headerRegex = "[.*";
    regcomp(&regex, headerRegex, 0);
    regmatch_t matches[1];

    int reti = regexec(&regex, reqContent, 1, matches, 0);

    if (reti != 0) {
        fprintf(stderr, "Did not request body");
        return -1;
    }

    size_t headerNameSize = strlen(headerName);
    int headerContentStart = matches[0].rm_so + headerNameSize + 2;
    int headerContentEnd = matches[0].rm_eo;


    int headerContentSize = headerContentEnd - headerContentStart;
    *output = malloc(headerContentSize + 1);
    snprintf(*output, headerContentSize, reqContent + headerContentStart);
    
    return headerContentSize;
}
