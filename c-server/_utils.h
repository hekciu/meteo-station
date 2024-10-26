#ifndef _UTILS_H
#define _UTILS_H

size_t build_response(char ** response);
size_t build_response_unauthorized(char ** response);
size_t build_response_not_found(char ** response);
size_t build_response_bad_request(char ** response, char * content);
int extract_header(char * reqContent, char * headerName, char ** output);
int extract_request_body(char * buffer, char ** output);
int extract_query_param(char * endpoint, const char * paramName, char ** output);

#endif
