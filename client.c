

#include <string.h>
#include <stdio.h>
#include <errno.h>

#include <stdbool.h>


#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>

#include <stdlib.h>

#include "socket.h"

int main(int argc, char* argv[]) {
    /*---VARIABLES---*/
    const char *hostname = argv[1];
    const char *servicename = argv[2];
    int e = 0;
    //int skt = 0;

    //bool are_we_connected = false;
    bool is_there_a_socket_error = false;

    socket_t socket;
    socket_init(&socket);
    socket_connect(&socket,hostname, servicename);


    char buf[3] = "";
    int size = 3;
    socket_receive(&socket, buf, size);
    int attemps = (int) buf[0] & 0x7F;

    
    unsigned short len = 0;
    memcpy(&len, &buf[1], 2);
    len = ntohs(len);

    
    char *rbuffer = calloc(sizeof(char), len+1);
    e = socket_receive(&socket, rbuffer, len);
    printf("Palabra secreta: %s\n", rbuffer);
    printf("Te quedan %d intentos\n", attemps);
    printf("Ingrese letra: ");
    free(rbuffer);
    bool end_game = false;
    while (!end_game) {
        /*---SEND---*/
    
        char buffer[100] = "";    
        e = scanf("%s", buffer);  
        if (e != 1) {
            return 1;
        }
        for (int i = 0; i < strlen(buffer) && !end_game; i++) {
            e = socket_send(&socket, &buffer[i], 1);
            if (e == -1) {
                printf("Error: %s\n", strerror(errno));
                is_there_a_socket_error = true;
                break;
            }

            /*---RECEIVE---*/
                char buf[3] = "";
                int size = 3;
                socket_receive(&socket, buf, size);
                int attemps = (int) buf[0] & 0x7F;
                int state = (int)buf[0] & 0x80;

                
                unsigned short len = 0;
                memcpy(&len, &buf[1], 2);
                len = ntohs(len);
        
                
                char *rbuffer = calloc(sizeof(char), len+1);
                e = socket_receive(&socket, rbuffer, len);

                if (state == 128) {
                    if (attemps == 0) {
                        printf("\nPerdiste! La palabra secreta era: '%s'", rbuffer);
                    
                        
                    } else {
                        printf("\nGanaste!!");
                    }
                    end_game = true;
                    free(rbuffer);
                    break;
                }
                printf("\nPalabra secreta: %s\n", rbuffer);
                printf("Te quedan %d intentos\n", attemps);
                printf("Ingrese letra: ");
                free(rbuffer);
            }
    }
    /*---ASVISA A LA OTRA MAQUINA QUE CIERRA CONEXION---*/
    socket_uninit(&socket);

    /*---FINISHING---*/
    if (is_there_a_socket_error) {
        return 1;  
    }
    else {
        return 0;
    }
}    
