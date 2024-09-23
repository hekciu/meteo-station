#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <regex.h>

size_t extract_header(char * reqContent, char * headerName, char ** output) {
    size_t outputSize = -1;

    if(*output != NULL) {
        free(*output);
        *output = NULL;
    } 

    regex_t regex;
    char * headerRegex = malloc(512 * sizeof(char)); // TODO: replace this magic number with BUFFER_SIZE global const
    sprintf(headerRegex, "%s: *\n", headerName);
    printf("regex body: %s\n", headerRegex);
    regcomp(&regex, headerRegex, REG_EXTENDED);
    regmatch_t matches[2];

    int reti = regexec(&regex, reqContent, 2, matches, 0);
    printf("reti: %d\n", reti);
    if (reti != 0) {
        fprintf(stderr, "Did not find requested header %s\n", headerName);
        return -1;
    }

    size_t reqSize = strlen(reqContent);
    printf("reqContent: %s\n\n\n", reqContent);
    
    // TODO: parse this text blob and find all them headers 
    return outputSize;
}
