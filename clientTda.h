#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <arpa/inet.h>

#include "socket.h"

typedef struct{
    socket_t socket;
} client_t;

void client_init_connect(client_t *self,
                     const char *hostname,
                         const char *servicename);

int client_receive_process(client_t *self,
                             char* buffer,
                                 size_t size,
                                     int ingame);

int client_send(client_t *self, char *letter, int size);

void client_uninit(client_t *self);
