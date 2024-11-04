#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

/*
    Makes json string array from n separate strings. Need to be called n + 1 times.
    At first this function should be called with pointer set to NULL and first element of array,
    after that it should be called with the same output pointer and next elements as input.
    To close the array, you should call this function with NULL as input. 
    User is also obligated to free output memory after usage.
*/
uint64_t jsonArrayFromStrings(char ** output, char * input) {
    uint64_t outputLen;

    if (*output == NULL && input != NULL) {
        outputLen = strlen(input) + 3 + 1;
        
        *output = malloc(outputLen);
        snprintf(*output, outputLen, "[\"%s\"", input);
    } else if (*output != NULL && input != NULL) {
        outputLen = strlen(*output) + strlen(input) + 4 + 1;

        char * tmp = malloc(strlen(*output) + 1);
        *(tmp + strlen(*output)) = '\0';
        memcpy(tmp, *output, strlen(*output));
        free(*output);
        *output = malloc(outputLen);

        snprintf(*output, outputLen, "%s,\"%s\"", tmp, input);

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
