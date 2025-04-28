#include <stdbool.h> // bool
#include <string.h>  // strncat, strcpy_s, strcat_s, strlen
#include <stdio.h> // perror()
#include <errno.h> // errno
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h> // open()

#include "path.h"

#define MAX_ARG_LEN 100
#define MAX_NUM_LEN 11

bool calc(char* path, char* output_txt, int length) {
    char operator[MAX_ARG_LEN];
    
    path = get_item_from_path(path, operator, MAX_ARG_LEN);
    printf("get_item returned operator: %s, new path %s\n", operator, path);
    fflush(stdout);
    if (strcmp("add", operator) != 0 && strcmp("mul", operator) != 0 && strcmp("div", operator) != 0) {
        printf("Invalid operator\n");
        fflush(stdout);
        return false;
    }
    
    char arg[MAX_NUM_LEN];
    path = get_item_from_path(path, arg, sizeof(arg));
    int num1 = atoi(arg);
    printf("get_item returned arg1: %s (%d), new path %s\n", arg, num1, path);
    fflush(stdout);
    path = get_item_from_path(path, arg, sizeof(arg));
    int num2 = atoi(arg);
    printf("get_item returned arg2: %s (%d), new path %s\n", arg, num2, path);
    fflush(stdout);

    int soln = 0;
    if (operator[0] == 'a') {soln = num1 + num2;}
    if (operator[0] == 'm') {soln = num1 * num2;}
    if (operator[0] == 'd') {soln = (int)(num1 / num2);}
    snprintf(output_txt, length, "%d\n", soln); // text output
    printf("solution int: %d, solution str: %s\n", soln, output_txt);
    fflush(stdout);
    return true;
}

bool stat(char* path, int output_fd) {
    /*
    takes the name of a file in a "/static" directory. Ex: "/static/images/rex.png", 
    returns the binary file that is there accessible to your server. (.bin is a binary file, .txt is a text file)
    */
    int end_ix = 0;
    char end_char = '\0';
    for (; end_ix < strlen(path); end_ix++){
        if (path[end_ix] == ' ' || path[end_ix] == '\n' || path[end_ix] == '\r') {
            end_char = path[end_ix];
            path[end_ix] = '\0';
        }
    }
    int input_fd;
    char buf[4096]; //TODO: remove magic number
    ssize_t nread;

    input_fd = open(path, O_RDONLY);
    if (input_fd < 0) {
        printf("Attempting to open file %s\n", path);
        perror("Unable to open file");
        path[end_ix] = end_char;
        return false;
    }
    path[end_ix] = end_char;
    
    while (nread = read(input_fd, buf, sizeof buf), nread > 0)
    {
        char *out_ptr = buf;
        ssize_t nwritten;

        do {
            nwritten = write(output_fd, out_ptr, nread);

            if (nwritten >= 0) {
                nread -= nwritten;
                out_ptr += nwritten;
            }
            else if (errno != EINTR) {
                close(input_fd);
                return false;
            }
        } while (nread > 0);
    }

    close(input_fd);
    if (nread == -1) {return false;}

    return true;
}
