#include <stdbool.h> // bool
#include <string.h>  // strncat, strcpy_s, strcat_s, strlen
#include <stdio.h> // printf()

#define MAX_ARG_LEN 100

bool is_request(char req[], int req_ix) {
    if (req_ix < strlen("GET ") || strncmp("GET ", req, strlen("GET ")) != 0) {return false;} // if request doesn't start with "GET "
    //if (req_ix < strlen(" HTTP/1.1") || strcmp(req + req_ix - strlen(" HTTP/1.1"), " HTTP/1.1") != 0) {return false;} // if request doesn't end with " HTTP/1.1"
    return true;
}

bool validate_path(char* path) {
    if (path[0] != '/') {
        printf("Invalid path. Missing '/' at start of path.");
        fflush(stdout);
        return false;
    }
    for (int ix = 0; ix < strlen(path); ix++) {
        if (ix < strlen(path)-1 && path[ix] == '/' && (path[ix+1] == '/' || path[ix+1] == ' ')) {
            printf("Invalid path. Missing argument.");
            fflush(stdout);
            return false;
        }
        if (ix < strlen(path)-1 && path[ix] == '.' && path[ix+1] == '.') {
            printf("Invalid path. Contains '..'");
            fflush(stdout);
            return false;
        }
        if (path[ix] == ' ') {break;}
    }
    return true;
}

const char* get_item_from_path(const char* path, char* item, int length) { 
    // returns a substring of the given path starting after the intial forward slash and stopping before the next forward slash or space (doesn't include the starting or ending characters)
    int item_len = 0;
    for (int ix = 1; ix < strlen(path); ix++) {
        if (path[ix] == '/' || path[ix] == ' ') {break;}
        item_len++;
    }
    
    strncpy(item, path+1, item_len < length ? item_len : length);
    item[length - 1] = '\0';

    return (path + 1 + item_len);
}