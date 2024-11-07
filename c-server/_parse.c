#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "_database.h"

#define PARSE_BUFFER_SIZE 1024


// TODO: maybe write some custom error messages to the response?
// TODO: create proper url decoding: https://www.ietf.org/rfc/rfc3986.txt
// TODO: write generic function for parsing url-encoded data (with callback)
int parseInsertPMS5003Body(char * body, uint64_t * device_timestamp, char ** device_name, uint16_t * pm10_standard, uint16_t * pm25_standard, uint16_t * pm100_standard) {
    int inputSize = strlen(body);
    char * currentParamNameBuffer = malloc(PARSE_BUFFER_SIZE);
    char * currentParamBuffer = malloc(PARSE_BUFFER_SIZE);
    int currentParamNameSize = 0;
    int currentParamSize = 0;
    char * errorBuffer = malloc(PARSE_BUFFER_SIZE);
    bool isCurrentlyName = true;
    int output = 0;
    const int numParams = 5;
    int paramsFound = 0;

    if (inputSize >= PARSE_BUFFER_SIZE) {
        fprintf(stderr, "input body exceeded parse buffer size\n");
        free(currentParamNameBuffer);
        free(currentParamBuffer);
        free(errorBuffer);
        return 1;
    }


    for (int n = 0; n <= inputSize; n++) {
        char charAtN = *(body + n); // At n == inputSize this will be \0

        if (charAtN == '&' || n == inputSize) {
            isCurrentlyName = true;
            *(currentParamBuffer + currentParamSize) = '\0';

            if (strncmp(currentParamNameBuffer, DEVICE_TIMESTAMP_FN, currentParamNameSize) == 0) {
                *device_timestamp = (uint64_t)strtoll(currentParamBuffer, &errorBuffer, 10);  
                paramsFound++;
            } else if (strncmp(currentParamNameBuffer, DEVICE_NAME_FN, currentParamSize) == 0) {
                *device_name = malloc(currentParamSize + 1);
                memcpy(*device_name, currentParamBuffer, currentParamSize);
                *(*device_name + currentParamSize) = '\0';
                paramsFound++;
            } else if (strncmp(currentParamNameBuffer, PM10_STANDARD_FN, currentParamSize) == 0) {
                *pm10_standard = (uint16_t)strtol(currentParamBuffer, &errorBuffer, 10);
                paramsFound++;
            } else if (strncmp(currentParamNameBuffer, PM25_STANDARD_FN, currentParamSize) == 0) {
                *pm25_standard = (uint16_t)strtol(currentParamBuffer, &errorBuffer, 10);
                paramsFound++;
            } else if (strncmp(currentParamNameBuffer, PM100_STANDARD_FN, currentParamSize) == 0) {
                *pm100_standard = (uint16_t)strtol(currentParamBuffer, &errorBuffer, 10);
                paramsFound++;
            } else {
                fprintf(stderr, "unsupported param in PMS5003 data: %s\n", currentParamNameBuffer);
                output = 1;
                break;
            }

            if (strlen(errorBuffer) != 0) {
                output = 1;
                break;
            } 

            currentParamSize = 0;
            continue; 
        }

        if (charAtN == '=') {
            isCurrentlyName = false;
            *(currentParamNameBuffer + currentParamNameSize) = '\0';
            currentParamNameSize = 0;
            continue;
        }

        if (isCurrentlyName) {
            *(currentParamNameBuffer + currentParamNameSize) = charAtN;
            currentParamNameSize++;
        } else {
            *(currentParamBuffer + currentParamSize) = charAtN;
            currentParamSize++;
        }
    }

    // free(currentParamBuffer);
    // free(currentParamNameBuffer);
    free(errorBuffer);

    if (paramsFound != numParams) {
        output = 1;
    }

    return output;
}
