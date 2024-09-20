#include <stdio.h>
#include <string.h>
#include <stdlib.h>


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
