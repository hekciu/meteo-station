#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <regex.h>
#include <stdbool.h>


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

    return (size_t)lengthWithoutNull; // TODO this conversion does not make sense, maybe switch to int
}


size_t build_response_bad_request(char ** response, char * content) {
    char * responseStart = "HTTP/1.1 400 Bad Request \r\n";
    char * headers =
        "Content-Type: text/html; charset=utf-8\r\n"
        "Accept-Ranges: bytes\r\n"
        "Connection: keep-alive\r\n\r\n";
    char * bodyContent = content == NULL ? "Bad Request" : content;
    size_t length = strlen(responseStart) + strlen(headers) + strlen(bodyContent) + 1;
    *response = malloc(length);
    int lengthWithoutNull = snprintf(*response, length, "%s%s%s", responseStart, headers, bodyContent);

    return (size_t)lengthWithoutNull;  // TODO this conversion does not make sense, maybe switch to int   
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

    return (size_t)lengthWithoutNull; // TODO this conversion does not make sense, maybe switch to int 
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
    return (size_t)lengthWithoutNull;// TODO this conversion does not make sense, maybe switch to int
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
        return 1;
    }

    size_t headerNameSize = strlen(headerName);
    int headerContentStart = matches[0].rm_so + headerNameSize + 2;
    int headerContentEnd = matches[0].rm_eo;


    int headerContentSize = headerContentEnd - headerContentStart;
    *output = malloc(headerContentSize + 1);
    snprintf(*output, headerContentSize, reqContent + headerContentStart);
    
    return 0;
}

int extract_request_body(char * reqContent, char ** output) {
    if(*output != NULL) {
        free(*output);
        *output = NULL;
    } 

    regex_t regex;
    char * bodyRegex = "\r\n\r\n.*";
    regcomp(&regex, bodyRegex, REG_EXTENDED);
    regmatch_t matches[1];

    int reti = regexec(&regex, reqContent, 1, matches, 0);

    if (reti != 0) {
        return 1;
    }

    int bodyStart = matches[0].rm_so + 4;
    int bodyEnd = matches[0].rm_eo;

    int bodySize = bodyEnd - bodyStart + 1;
    *output = malloc(bodySize);
    snprintf(*output, bodySize, reqContent + bodyStart);
    
    return 0;
}

int extract_query_param(char * endpoint, char * paramName, char ** output) {
    int endpointLen = strlen(endpoint);
    int paramNameLen = strlen(paramName);

    int paramNamePatternLen = paramNameLen + 1;
    char * paramNamePattern = malloc(paramNameLen + 1);
    sprintf(paramNamePattern, "%s=", paramName);

    bool foundFirstParam = false;
    int paramStartIndex = -1;
    int paramContentLen = 0; 
    for (int i = 0; i < endpointLen; i++) {
        char charAtI = *(endpoint + i);

        if (paramStartIndex > 0) {
            if (charAtI == '&') {
                break;
            }

            paramContentLen++;
            continue;
        }

        if (!foundFirstParam && charAtI == '?') {
            if (strncmp(endpoint + i + 1, paramNamePattern, paramNameLen) == 0) {
                paramStartIndex = i + 1; 
            } 
        } 

        if (foundFirstParam && charAtI == '&') {
            if (strncmp(endpoint + i + 1, paramNamePattern, paramNameLen) == 0) {
                paramStartIndex = i + 1; 
            } 
        } 

        if (charAtI == '?') {
            foundFirstParam = true;
        }
    } 

    free(paramNamePattern);

    if (paramStartIndex > 0 && paramContentLen > 0) {
        int outputLen = paramContentLen - paramNamePatternLen;
        *output = malloc(outputLen + 1);
        
        memcpy(*output, endpoint + paramNamePatternLen + paramStartIndex, outputLen); 
        *(*output + outputLen) = '\0';
        return 0;
    }

    return 1;
}
