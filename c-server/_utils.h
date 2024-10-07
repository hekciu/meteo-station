#ifndef _UTILS_H
#define _UTILS_H

size_t build_response(char ** response);
size_t build_response_unauthorized(char ** response);
size_t build_response_not_found(char ** response);
int extract_header(char * reqContent, char * headerName, char ** output);
size_t extract_request_body(char * buffer, char ** output);

#endif
