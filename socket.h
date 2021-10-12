#ifndef SOCKET_H
#define SOCKET_H

#include <stddef.h>
#include <stdint.h>
#include <unistd.h>

/* fd: file descriptor of the socket*/
typedef struct {
    int fd;
} socket_t;
/*creates socket*/
void socket_init(socket_t *self);
/*destroys socket*/
void socket_uninit(socket_t *self);
/*binds and puts a socket to listen*/
void socket_bind_and_listen(socket_t *self,
                            const char *host,
                            const char *service);
/*accepts a socket*/
void socket_accept(socket_t *listener, socket_t *peer);
/*connects a socket*/
void socket_connect(socket_t *self, const char *host, const char *service);
/*sends buffer*/
ssize_t socket_send(socket_t *self, const char *buffer, size_t length);
/*receives buffer*/
ssize_t socket_receive(socket_t *self, char *buffer, size_t length);

#endif


