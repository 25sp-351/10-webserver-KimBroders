#include <stdio.h>      // printf, printf_s, scanf_s, gets, NULL
#include <stdlib.h>     // malloc, free, rand, srand, exit, _sleep
#include <string.h>     // strcpy_s, strcat_s, strlen
#include <unistd.h>
#include <stdbool.h> // bool
#include <ctype.h> //isprint()

#include "path.h"
#include "request.h"

#define MAX_MESSAGE_SIZE 10000
#define MAX_NUM_LEN 11

bool terminal_print = false;

void handleConnection(int* sock_fd_ptr)
{
    int sock_fd = *sock_fd_ptr;
    printf("Handling connection on %d\n", sock_fd);
    
    // handle a message
    char buf;
    char msg [MAX_MESSAGE_SIZE];
    memset(msg, 0, sizeof(msg));
    int msg_index = 0;
    while (1) {
        int chars = read(sock_fd, &buf, sizeof(buf)); // read the input data one character at a time
        //printf("%d charcters read: %d (%c:%d) msg_index: %d\n", sock_fd, chars, isprint(buf) ? buf : '?', buf, msg_index);
        //fflush(stdout);
        if (chars == 0 || chars == -1) { // break if connection is closed
            printf("%d read returned: %d\n", sock_fd, chars);
            fflush(stdout);
            close(sock_fd);
            break;
        }
        
        // store the message
        msg[msg_index++] = buf;
        if (buf == '\n' || msg_index == MAX_MESSAGE_SIZE) {
            if (terminal_print == true) {
                printf("%d received: (%d bytes) ", sock_fd, msg_index); // print to the terminal
                for (unsigned ix = 0U; ix < msg_index; ++ix) printf("%c", isprint(msg[ix]) || msg[ix] == '\r' || msg[ix] == '\n' ? msg[ix] : '?');
                fflush(stdout);
            }
            if (is_request(msg, msg_index)) { // check that the request starts with "GET " and ends with " HTTP/1.1"
                char* msg_ptr = msg;
                msg_ptr += strlen("GET ");
                if (!validate_path(msg_ptr)) {
                    printf("Invalid path\n");
                    fflush(stdout);
                }
                else{
                    char call_funct[MAX_MESSAGE_SIZE];
                    get_item_from_path(msg_ptr, call_funct, MAX_MESSAGE_SIZE);
                    fflush(stdout);
                    if (strcmp("calc", call_funct) == 0) {
                        msg_ptr += strlen("/calc");
                        char soln_string[MAX_NUM_LEN];
                        calc(msg_ptr, soln_string, MAX_NUM_LEN);
                        if (terminal_print) printf("calculation solution: %s", soln_string);
                        write(sock_fd, soln_string, strlen(soln_string)); // write the input data
                    }
                    else if (strncmp("/static", msg_ptr, strlen("/static")) == 0) {
                        msg_ptr += strlen("/static");
                        msg_ptr++;
                        stat(msg_ptr, sock_fd);
                    }
                    else {
                        if (terminal_print) printf("unknown msg\n");
                    }
                }
            }
            else {
                if (terminal_print == true) {
                    printf("%d Not a request, ignoring msg\n", sock_fd); // print to the terminal
                    fflush(stdout);
                }
            }
            msg_index = 0;
            memset(msg, 0, sizeof(msg));
        }
    }
    printf("done with connection %d\n", sock_fd);
}