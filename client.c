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
#include "calculator.h"

int main(int argc, char* argv[]) {

    /*------VARIABLES socket--------*/
    int s = 0;                                                                           
    int skt;
    bool is_there_a_socket_error = false;
    struct addrinfo hints;
    struct addrinfo *result;                                                                                          
    struct addrinfo *ptr;
    char *port = argv[1];
    char *node = argv[2];

    /*------GETADDRINFO--------*/
    memset(&hints, 0, sizeof(struct addrinfo));                                                                                                                                                             
    hints.ai_family = AF_INET;                                                                                             
    hints.ai_socktype = SOCK_STREAM;                                                                                       
                                                                                         
    s = getaddrinfo(node, port, &hints, &result);
    if (s != 0) {
      printf("Error in getaddrinfo: %s\n", gai_strerror(s));
      return 1;
    }

    /*------GETADDRINFO ELIJO DIRECCIÓN--------*/
    socket_t socket;

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

    /*------PRIMER MENSAJE DEL SERVIDOR CON JUEGO--------*/
    char recvBuffer[40] = "";
    s = socket_receive(&socket, recvBuffer, sizeof(recvBuffer));
    if (s == -1) {
        return 1;
    } 
    if (s == 1) { 
        printf("CLIENT SOCKET CLOSED.\n");
        return 1;
    } else {
        printf("MENSAJE RECIBIDO DEL SERVER: %s\n", recvBuffer);
    }

    /*------SENDING--------*/
    while(1) {
        char recvBuffer[40] = "";
        char buffer[1] = "";    
        scanf("%s", buffer);
        
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

        s = socket_receive(&socket, recvBuffer, sizeof(recvBuffer));
        if (s == -1) {
            return 1;
        } 
        if (s == 1) { 
            printf("CLIENT SOCKET CLOSED.\n");
            break;
        } else {
            printf("MENSAJE RECIBIDO DEL SERVER: %s\n", recvBuffer);
        }

    }
    close(skt);                                                                                                    
    printf("CLIENT SOCKET CLOSED\n");

    return 0;
}



