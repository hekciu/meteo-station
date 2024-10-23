#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

/*
    This function differs from jsonArrayFromStrings in a manner that it does not insert parenthesis around input strings
*/
uint64_t jsonArrayFromAnything(char ** output, char * input) {
    uint64_t outputLen;

    if (*output == NULL && input != NULL) {
        outputLen = strlen(input) + 2;
        
        *output = malloc(outputLen);
        snprintf(*output, outputLen, "[%s", input);
    } else if (*output != NULL && input != NULL) {
        uint64_t newContentLen = strlen(input) + 2;
        outputLen = strlen(*output) + newContentLen;

        realloc(*output, newContentLen);
        snprintf(*output + strlen(*output), newContentLen, ",%s", input);
    } else if (input == NULL) {
        realloc(*output, 2);
        snprintf(*output + strlen(*output), 2, "]");
    }

    return outputLen;
}
