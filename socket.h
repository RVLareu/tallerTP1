#ifndef SOCKET_H
#define SOCKET_H
#include <netdb.h> 
#include <stddef.h>
#include <stdint.h>
#include <unistd.h>


typedef struct {
    int fd;
} socket_t;

int socket_init(socket_t *self, struct addrinfo *ptr); 
void socket_uninit(socket_t *self);

int socket_bind_and_listen(socket_t *self, struct addrinfo *ptr); 
int socket_accept(socket_t *listener, socket_t *peer);

int socket_connect(socket_t *self, struct addrinfo *ptr);

ssize_t socket_send(socket_t *self, const char *buffer, size_t length);
ssize_t socket_receive(socket_t *self, char *buffer, size_t length);

#endif