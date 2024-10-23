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
        outputLen = strlen(input) + 4;
        
        *output = malloc(outputLen);
        snprintf(*output, outputLen, "[\"%s\"", input);
    } else if (*output != NULL && input != NULL) {
        uint64_t newContentLen = strlen(input) + 4;
        outputLen = strlen(*output) + newContentLen;

        realloc(*output, newContentLen);
        snprintf(*output + strlen(*output), newContentLen, ",\"%s\"", input);
    } else if (input == NULL) {
        realloc(*output, 2);
        snprintf(*output + strlen(*output), 2, "]");
    }

    return outputLen;
}
