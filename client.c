

#include <string.h>
#include <stdio.h>
#include <errno.h>

#include <stdbool.h>


#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "socket.h"

int main(int argc, char* argv[]) {
    /*---VARIABLES---*/
    const char *hostname = argv[1];
    const char *servicename = argv[2];
    int e = 0;
    int skt = 0;

    bool are_we_connected = false;
    bool is_there_a_socket_error = false;

    struct addrinfo hints;
    struct addrinfo *result, *ptr;

    /*---GETADDRINFO---*/
    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_INET;       
    hints.ai_socktype = SOCK_STREAM; 
    hints.ai_flags = 0;

    e = getaddrinfo(hostname, servicename, &hints, &result);   
    if (e != 0) { 
        printf("Error in getaddrinfo: %s\n", gai_strerror(e));
        return 1;
    }
    /*---ITERO LISTA GETADDRINFO, SOCKET, CONNECT---*/
    for (ptr = result; ptr != NULL && are_we_connected == false; ptr = ptr->ai_next) {
        skt = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
        if (skt == -1) {
            printf("Error: %s\n", strerror(errno));
        } else {
            e = connect(skt, ptr->ai_addr, ptr->ai_addrlen);
            if (e == -1) {
                printf("Error: %s\n", strerror(errno));
                close(skt);
            }
            are_we_connected = (e != -1);
        }
    }

    /*---FREE ADDRINFO---*/
    freeaddrinfo(result);

    /*---NO CONECTÓ A NINGUNO DE LA LISTA---*/
    if (are_we_connected == false) {
     return 1;
    }
    while (1) {
    /*---RECEIVE---*/
        char buf[3] = "";
        int size = 3;
        recv_message(skt, buf, size);
        int attemps = (int) buf[0] & 0x7F;
        int state = (int)buf[0] & 0x80;

        
        unsigned short len = 0;
        memcpy(&len, &buf[1], 2);
        len = ntohs(len);
  
        
        char *rbuffer = calloc(sizeof(char), len+1);
        e = recv_message(skt, rbuffer, len);

        if (state == 128) {
            if (attemps == 0) {
                printf("Perdiste! La palabra secreta era: '%s'", rbuffer);
                
                
            } else {
                printf("Ganaste!!");
            }
            free(rbuffer);
            break;
        }
        printf("Palabra secreta : %s\n", rbuffer);
        printf("Te quedan %d intentos\n", attemps);
        printf("Ingrese letra: ");
        free(rbuffer);



        /*---SEND---*/
    
        char buffer[1] = "";    
        e = scanf("%s", buffer);  
        if (e != 1) {
            return 1;
        }      
        e = send_message(skt, buffer, 1);
        if (e == -1) {
            printf("Error: %s\n", strerror(errno));
            is_there_a_socket_error = true;
            break;
        }
    }
    /*---ASVISA A LA OTRA MAQUINA QUE CIERRA CONEXION---*/
    shutdown(skt, SHUT_RDWR);
    close(skt);

    /*---FINISHING---*/
    if (is_there_a_socket_error) {
        return 1;  
    }
    else {
        return 0;
    }
}    
