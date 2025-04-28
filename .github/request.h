#ifndef REQUEST_H
#define REQUEST_H

#include <stdbool.h> // bool
#include <string.h>  // strncat, strcpy_s, strcat_s, strlen

#define MAX_ARG_LEN 100
#define MAX_NUM_LEN 11

bool calc(char* path, char* output_txt, int length);

bool stat(char* path, int output_fd);

#endif // REQUEST_H