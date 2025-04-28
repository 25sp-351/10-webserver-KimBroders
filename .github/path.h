#ifndef PATH_H
#define PATH_H

#include <stdbool.h> // bool

bool is_request(char req[], int req_ix);
bool validate_path(char* path);
char* get_item_from_path(char* path, char* item, int length);

#endif // PATH_H