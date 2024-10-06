#ifndef _UTILS_H
#define _UTILS_H

size_t build_response(char ** response);
size_t extract_header(char * reqContent, char * headerName, char ** output);

#endif
