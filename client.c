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

    /*------VARIABLES--------*/
    int s = 0;                                                                           
    int skt;
    bool is_there_a_socket_error = false;
    struct addrinfo hints;
    struct addrinfo *result;                                                                                          
    struct addrinfo *ptr;

    /*------GETADDRINFO--------*/
    memset(&hints, 0, sizeof(struct addrinfo));                                                                                                                                                             
    hints.ai_family = AF_INET;                                                                                             
    hints.ai_socktype = SOCK_STREAM;                                                                                       
                                                                                         
    s = getaddrinfo(argv[2], argv[1], &hints, &result);                                                 
    if (s != 0) {
      printf("Error in getaddrinfo: %s\n", gai_strerror(s));
      return 1;
    }

    socket_t socket;
    /*------GETADDRINFO ELIJO DIRECCIÓN--------*/
    for (ptr = result; ptr != NULL; ptr = ptr->ai_next) { 
        
        skt = socket_init(&socket, ptr);                                                    
        if (skt == -1) {
            printf("Error: %s\n", strerror(errno));
        } else{
            s = socket_connect(&socket, ptr);
            if (s == -1) {
                printf("Error: %s\n", strerror(errno));
                close(skt);
            }
        }                                                                                                                                 
    }


    freeaddrinfo(result);
    printf("CLIENT SOCKET CONNECTED.\n");
    

    /*------SENDING--------*/
    while(1) {
        char buffer[10] = "";                          
        scanf("%s", buffer);
        //si no inicializo el buffer, valgrind se queja de unitialized bytes
        ssize_t s = socket_send(&socket, buffer, sizeof(buffer));
        if (s == -1) {
            printf("Error: %s\n", strerror(errno));
            is_there_a_socket_error = true;
        }
        if (is_there_a_socket_error) {
            shutdown(skt, SHUT_RDWR);
            close(skt);
            return 1;
        } else {
            printf("MENSAJE ENVIADO CORRECTAMENTE\n");
        }

    }
    close(skt);                                                                                                    
    printf("CLIENT SOCKET CLOSED\n");

    return 0;
}
