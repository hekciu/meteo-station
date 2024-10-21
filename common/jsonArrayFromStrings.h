#include <stdint.h>
#include <string.h>

/*
    At first this function should be called with pointer set to NULL and first element of array,
    after that it should be called with the same output pointer and next elements as input.
    To close the array, you should call this function with NULL as input. 
    User is also obligate to free output memory after usage.
*/
uint64_t jsonArrayFromStrings(char ** output, char * input) {
    uint64_t outputLen;

    if (*output == NULL && input != NULL) {
        outputLen = strlen(input) + 4;
        
        *output = malloc(outputLen);
        snprintf(*output, outputLen, "[\"%s\"", input);
    } else if (*output != NULL && input == NULL) {
        uint64_t newContentLen = 
        outputLen = strlen(*output) + strlen(input) + 4;

        char * newElementChunk = realloc(*output,      
    }

    return outputLen;
}
