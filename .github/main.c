#include <arpa/inet.h>
#include <netinet/in.h>
#include <pthread.h>    // pthread_t: Represents thread IDs., pthread_create, pthread_join
#include <stdio.h>      // printf, printf_s, scanf_s, gets, NULL
#include <stdlib.h>     // malloc, free, rand, srand, exit, _sleep
#include <string.h>     // strcpy_s, strcat_s, strlen
#include <sys/socket.h> // socket(), bind(), listen()
#include <unistd.h>
#include <stdbool.h> // bool

#include "handle_connection.h"

#define DEFAULT_PORT 80
#define LISTEN_BACKLOG 10

/*
Create a server that listens on a port (default: 80, changeable with a "-p" option).

Accept HTTP/1.1. One message at a time, no pipelining. 

Always:
    set the content-type appropriately when possible
    set the content-length correctly
    Check the method (should be GET for both of these)

Implement "/static" which allows the requester to specify the name of a file in a "/static" directory. Ex: "/static/images/rex.png", returns the binary file that is there accessible to your server.

Implement "/calc" which returns text or HTML, operating on the path components in the request
    /calc/add/<num1>/<num2>
    /calc/mul/<num1>/<num2>
    /calc/div/<num1>/<num2>

Make it multithreaded!

Test by hitting it from your browser. And from telnet. And from Postman or ThunderClient.

DESIGN GOALS (as in "I'm expecting this")
    Modules for requests and responses
    clean interfaces!

Extra credit: (25) Implement pipelining.
    can receive multiple client requests before server response is generated
    (implement a "/sleep/<seconds>" path to test this)
*/


int main(int argc, char* argv[])
{
    int port = DEFAULT_PORT;
    for (int ix = 1; ix < argc; ix++) {
        if(strlen(argv[ix]) == 2 && argv[ix][0] == '-') {
            if (argv[ix][1] == 'p' && argc > ix+1) {port = atoi(argv[++ix]);}
            if (argv[ix][1] == 'v') {terminal_print = true;}
        }
    }
    
    // Create and bind listening socket
    // socket()
    int socket_fd = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in socket_address;
    memset(&socket_address, '\0', sizeof(socket_address));
    socket_address.sin_family = AF_INET;
    socket_address.sin_addr.s_addr = htonl(INADDR_ANY);
    socket_address.sin_port = htons(port);

    printf("Binding to port %d\n", port);

    int returnval;
    
    // bind()
    returnval = bind(socket_fd, (struct sockaddr*)&socket_address, sizeof(socket_address));
    if (returnval < 0) {
        perror("bind");
        return 1;
    }
    
    // listen()
    returnval = listen(socket_fd, LISTEN_BACKLOG); 

    // Accept
    struct sockaddr_in client_address;
    socklen_t client_address_len = sizeof(client_address);
    
    while (1) {
        pthread_t thread;
        int* client_fb_buf = malloc(sizeof(int));
        
        // accept()
        *client_fb_buf = accept(socket_fd, (struct sockaddr*)&client_address, &client_address_len);
        
        printf("accepted connection on %d\n", *client_fb_buf);
        // Handle the connection
        pthread_create(&thread, NULL, (void* (*)(void*))handleConnection, (void*)client_fb_buf);
    }
    return 0;
}