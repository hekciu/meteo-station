#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <regex.h>

size_t extract_header(char * reqContent, char * headerName, char ** output) {
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
