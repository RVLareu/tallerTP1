#include <sys/types.h>                                                                                                     
#include <sys/socket.h>                                                                                                     
#include <netdb.h>                                                                                                         
#include <string.h>                                                                                                         
#include <stdio.h>                                                                                                         
#include <stdlib.h>                                                                                                         
#include <unistd.h>
#include <errno.h>                                                                                                         
#include <stdbool.h>

#include "socket.h"

int main(int argc, char* argv[]) {
    
    /*------VARIABLES SCOKET--------*/
    int s = 0, skt, peerskt;                                                                                                                                                                                                                                                                      
    int val = 1;
    struct addrinfo hints, *ptr;
    bool continue_running = true;
    bool is_the_accept_socket_valid = true;    



    /*------GETADDRINFO--------*/
    memset(&hints, 0, sizeof(struct addrinfo));  
    hints.ai_family = AF_INET;                                                                                            
    hints.ai_socktype = SOCK_STREAM;                                                                                    
    hints.ai_flags = AI_PASSIVE;          


    s = getaddrinfo(NULL, argv[1], &hints, &ptr);                                                   
    if (s != 0) {
      printf("Error in getaddrinfo: %s\n", gai_strerror(s));
      return 1;
    }

    /*------SOCKET INIT BIND LISTEN--------*/
    socket_t socket;
    skt = socket_init(&socket, ptr);
    socket_bind_and_listen(&socket, ptr);                                           

    freeaddrinfo(ptr);
    printf("LISTENING... en port: %s\n", argv[1]);
    



    /*------ACCEPTING--------*/
    while (continue_running) {
        socket_t peersocket;
        int peerskt = socket_accept(&socket, &peersocket);                                                                                   
        if (peerskt == -1) {
            continue_running = false;
            is_the_accept_socket_valid = false;
        }
        printf("CLIENT SOCKET ACCEPTED.\n");
        while(1) {
            char buffer[10];
            s = socket_receive(&peersocket, buffer, sizeof(buffer));
            if (s == -1) {
                return 1;
            } 
            if (s == 1) { 
                printf("CLIENT SOCKET CLOSED.\n");
                break;
            }
            printf("MESSAGE RECEIVED \n%s\n", buffer);
        }
    }
    /*------FIN--------*/
    socket_uninit(&socket);
    if (is_the_accept_socket_valid) {
        return 1;
    }
    else {
        return 0;
    }
}