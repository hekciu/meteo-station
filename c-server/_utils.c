#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <regex.h>


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

    regex_t regex;
    char * headerRegex = malloc(512 * sizeof(char)); // TODO: replace this magic number with BUFFER_SIZE global const
    sprintf(headerRegex, "%s: .+\n", headerName);
    regcomp(&regex, headerRegex, REG_EXTENDED);
    regmatch_t matches[2];

    int reti = regexec(&regex, reqContent, 2, matches, 0);

    if (reti != 0) {
        fprintf(stderr, "Did not find requested header %s\n", headerName);
        return -1;
    }

    size_t headerNameSize = strlen(headerName);
    int headerContentStart = matches[0].rm_so + headerNameSize + 2;
    int headerContentEnd = matches[0].rm_eo;

    int headerContentSize = headerContentEnd - headerContentStart;
    char * headerContent = malloc(headerContentSize + 1);
    snprintf(headerContent, headerContentSize, reqContent + headerContentStart);
    *(headerContent + headerContentSize) = '\0';
    
    return headerContentSize;
}
