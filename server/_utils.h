#ifndef _UTILS_H
#define _UTILS_H

extern size_t build_response(char ** response, char * content);
extern size_t build_response_unauthorized(char ** response);
extern size_t build_response_not_found(char ** response);
extern size_t build_response_bad_request(char ** response, char * content);
extern size_t build_response_internal_server_error(char ** response, char * content);
extern int extract_header(char * reqContent, char * headerName, char ** output);
extern int extract_request_body(char * buffer, char ** output);
extern int extract_query_param(char * endpoint, const char * paramName, char ** output);

#endif
