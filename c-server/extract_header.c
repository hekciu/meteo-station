#include <string.h>

int64_t extract_header(char * reqContent, char * headerName, char ** output) {
    if(*output != NULL) {
        free(*output);
        *output = NULL;
    }   

    // TODO: parse this text blob and find all them headers 
}
