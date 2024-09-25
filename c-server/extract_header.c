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
    sprintf(headerRegex, "%s: .+\n", headerName);
    regcomp(&regex, headerRegex, REG_EXTENDED);
    regmatch_t matches[2];

    int reti = regexec(&regex, reqContent, 2, matches, 0);

    if (reti != 0) {
        fprintf(stderr, "Did not find requested header %s\n", headerName);
        return -1;
    }

    size_t headerContentSize = matches[0].rm_eo - matches[0].rm_so;
    char * headerContent = malloc(headerContentSize + 1);
    snprintf(headerContent, headerContentSize, reqContent + matches[0].rm_so);
    printf("header content: %s\n", headerContent);
    
    // TODO: parse this text blob and find all them headers 
    return outputSize;
}
