#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "_database.h"

#define PARSE_BUFFER_SIZE 1024


// TODO: maybe write some custom error messages to the response?

int parseInsertPMS5003Body(char * body, uint64_t * device_timestamp, char ** device_name, uint16_t * pm10_standard, uint16_t * pm25_standard, uint16_t * pm100_standard) {
    int inputSize = strlen(body);
    bool foundOpeningBracket = false;
    bool foundClosingBracket = false;
    int currentParamSize = -1;
    int paramsFound = 0;
    char * currentParamBuffer = malloc(PARSE_BUFFER_SIZE);

    for (int n = 0; n < inputSize; ++i) {
        char charAtN = *(body + n);

        if (!foundOpeningBracket && charAtN != '{') {
            continue;
        } else {
            foundOpeningBracket = true;
        }

        if (charAtN == '}') {
            foundClosingBracket = true;
            break; 
        }

        if (currentParamSize == -1 && charAtN == ' ') {
            continue;
        }

        if (currentParamSize == -1 && charAtN != ' ' && charAtN != '"') {
            free(currentParamBuffer);
            return 1;
        }

        if (currentParamSize == -1 && charAtN == '"') {
            currentParamSize++;
            continue;
        }

        if (currentParamSize >= PARSE_BUFFER_SIZE) {
            fprintf(stderr, "Body field name size exceeded buffer limit\n")
            free(currentParamBuffer);
            return -1;
        }

        if (currentParamSize > -1 && charAtN != '"') {
            *(currentParamBuffer + currentParamSize) = charAtN;
            currentParamSize++;
        }

        if (currentParamSize > -1 && charAtN == '"') {
            *(currentParamBuffer + currentParamSize) = '\0';
            char * errorBuffer = malloc(PARSE_BUFFER_SIZE);

            if (strncmp(currentParamBuffer, DEVICE_TIMESTAMP_FN, currentParamSize) == 0) {
                
            } else if (strncmp(currentParamBuffer, DEVICE_NAME_FN, currentParamSize) == 0) {

            } else if (strncmp(currentParamBuffer, PM10_STANDARD_FN, currentParamSize) == 0) {

            } else if (strncmp(currentParamBuffer, PM25_STANDARD_FN, currentParamSize) == 0) {

            } else if (strncmp(currentParamBuffer, PM100_STANDARD_FN, currentParamSize) == 0) {

            } else {
                fprintf(stderr, "Unsupported field in PMS5003 data: %s\n", currentParamBuffer);
                free(currentParamBuffer);
                return 1;
            }

            currentParamSize = -1;
        }
    }

    free(currentParamBuffer);

    if (!foundOpeningBracket || !foundClosingBracket || paramsFound != 5) {
        return 1;
    }

    return 0;
}
