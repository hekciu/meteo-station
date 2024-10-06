#include <string.h>
#include <stdio.h>
#include "../common/base64_encode.h"

int auth(char * authHeaderContent) {
    char * username = getenv("THIS_API_USERNAME");
    char * password = getenv("THIS_API_PASSWORD"); 

    if (username == NULL || password == NULL) {
        fprintf(stderr, "Missing THIS_API_USERNAME or THIS_API_PASSWORD env variable, could not perform auth\n");
        return -1;
    }


    size_t plainAuthStrSize = strlen(username) + strlen(password) + 1 + 1; 
    char * plainAuthStr = malloc(plainAuthStrSize);
    snprintf(plainAuthStr, plainAuthStrSize, "%s:%s", username, password);

    char * encodedAuthStr = NULL;
    base64_encode(plainAuthStr, &encodedAuthStr);
     
    char * BASIC_AUTH_BEGINNING = "Basic ";
    size_t encodedAuthContentSize = strlen(BASIC_AUTH_BEGINNING) + strlen(encodedAuthStr) + 1;
    char * encodedAuthContent = malloc(encodedAuthContentSize);

    snprintf(encodedAuthContent ,encodedAuthContentSize, "%s%s", BASIC_AUTH_BEGINNING, encodedAuthStr);

    int isAuthorized = strcmp(authHeaderContent, encodedAuthContent);  

    free(encodedAuthStr);
    free(encodedAuthContent);
    free(plainAuthStr);

    return isAuthorized;
}
