#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <arpa/inet.h>

#include "socket.h"
/*socket: client socket*/
typedef struct{
    socket_t socket;
} client_t;
/*initiates client*/
void client_init_connect(client_t *self,
                     const char *hostname,
                         const char *servicename);
/*receives and process message from server*/
int client_receive_process(client_t *self,
                             char* buffer,
                                 size_t size,
                                     int ingame);
/*sends a message to server*/
int client_send(client_t *self, char *letter, int size);
/*destroys client*/
void client_uninit(client_t *self);
