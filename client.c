
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

    int bytes_receive = 0;
    char attemp;
    while (bytes_receive < 1) {
        s = socket_receive(&socket, &attemp, sizeof(unsigned char));
        if (s == -1) {
            printf("Error: %s\n", strerror(errno));
        } else {
            bytes_receive += 1;
        }
    }
    int a = (int) attemp & 0x7F;
    

    bytes_receive = 0;
    char len1;
    char len2;
    while (bytes_receive < 1) {
        s = socket_receive(&socket, &len1, sizeof(char));
        if (s == -1) {
            printf("Error: %s\n", strerror(errno));
        } else {
            bytes_receive += 1;
        }
    }
    bytes_receive = 0;
    while (bytes_receive < 1) {
        s = socket_receive(&socket, &len2, sizeof(char));
        if (s == -1) {
            printf("Error: %s\n", strerror(errno));
        } else {
            bytes_receive += 1;
        }
    }
    short len = (((short) len1)<<8) | len2;

    char word[len];
    bytes_receive = 0;
    while (bytes_receive < len) {
        s = socket_receive(&socket, &word[bytes_receive], sizeof(char));
        if (s == -1) {
            printf("Error: %s\n", strerror(errno));
        } else {
            bytes_receive += 1;
        }
    }
    printf("Palabra secreta: %s\n", word);
    printf("Te quedan %d intentos\n", a);
    printf("Ingrese letra:");

    /*------SENDING--------*/
    bool playing = true;
    while(playing) {

        char buffer[1] = "";    
        int error = scanf("%s", buffer);
        if (error != 2) {
            return 1;
        }
            
        
        
        for (int i = 0; i < strlen(buffer); i++) {
            int bytes_sent = 0;
            while (bytes_sent < 1) {
                ssize_t s = socket_send(&socket, &buffer[i], sizeof(char));
                if (s == -1) {
                    printf("Error: %s\n", strerror(errno));
                    is_there_a_socket_error = true;
                }
                if (is_there_a_socket_error) {
                    socket_uninit(&socket);  
                    return 1;
                } else {
                    bytes_sent += 1;
                }
            }




            char attemp;
            bytes_receive = 0;
            while (bytes_receive < 1) {
                s = socket_receive(&socket, &attemp, sizeof(unsigned char));
                if (s == -1) {
                    printf("Error: %s\n", strerror(errno));
                } else {
                    bytes_receive += 1;
                }
            }
            bytes_receive = 0;
            char len1;
            char len2;
            while (bytes_receive < 1) {
                s = socket_receive(&socket, &len1, sizeof(char));
                if (s == -1) {
                    printf("Error: %s\n", strerror(errno));
                } else {
                    bytes_receive += 1;
                }
            }
            bytes_receive = 0;
            while (bytes_receive < 1) {
                s = socket_receive(&socket, &len2, sizeof(char));
                if (s == -1) {
                    printf("Error: %s\n", strerror(errno));
                } else {
                    bytes_receive += 1;
                }
            }
            short len = (((short) len1)<<8) | len2;

            char word[len];
            bytes_receive = 0;
            while (bytes_receive < len) {
                s = socket_receive(&socket, &word[bytes_receive], sizeof(char));
                if (s == -1) {
                    printf("Error: %s\n", strerror(errno));
                } else {
                    bytes_receive += 1;
                }
            }           
            int b = (int) attemp & 0x80;
            int a = (int) attemp & 0x7F;
            if (b != 0) {
                if (a == 0){
                    printf("Perdiste! La palabra secreta era: '%s'\n", word);
                    playing = false;
                    break;
                    
                } else{
                    printf("Ganaste!!\n");
                    playing = false;
                    break;
                    
                }

            }
            


            printf("Palabra secreta: %s\n", word);
            printf("Te quedan %d intentos\n", a);
            printf("Ingrese letra:");
        }
    }
    socket_uninit(&socket);                     

    return 0;
}



