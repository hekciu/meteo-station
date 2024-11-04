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
        outputLen = strlen(input) + 1 + 1;

        *output = malloc(outputLen);
        
        snprintf(*output, outputLen, "[%s", input);
    } else if (*output != NULL && input != NULL) {
        outputLen = strlen(*output) + strlen(input) + 2 + 1;

        char * tmp = malloc(strlen(*output) + 1);
        *(tmp + strlen(*output)) = '\0';
        memcpy(tmp, *output, strlen(*output));
        free(*output);
        *output = malloc(outputLen);

        snprintf(*output, outputLen, "%s,%s", tmp, input);

        free(tmp);
    } else if (input == NULL) {
        outputLen = strlen(*output) + 2;

        char * tmp = malloc(strlen(*output) + 1);
        memcpy(tmp, *output, strlen(*output));
        *(tmp + strlen(*output)) = '\0';
        free(*output);
        *output = malloc(outputLen);

        snprintf(*output, outputLen, "%s]", tmp);

        free(tmp);
    }

    return outputLen;
}
