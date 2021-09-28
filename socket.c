#include <sys/types.h>
#include<sys/socket.h>
#include <stdio.h> 
#include <errno.h>
#include <string.h>  
#include "socket.h"

#define PEERS_WAITING_MAX 8

int socket_init(socket_t *self, struct addrinfo *ptr){
    int val = 1;
    int s;
    self->fd = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
    if (self->fd == -1) {
        printf("Error: %s\n", strerror(errno));
        freeaddrinfo(ptr);
        return 1;
    }
    s = setsockopt(self->fd, SOL_SOCKET, SO_REUSEADDR, &val, sizeof(val));
    if (s == -1) {
        printf("Error: %s\n", strerror(errno));
        close(self->fd);
        freeaddrinfo(ptr);
        return 1;
    }
    return 0;

}
void socket_uninit(socket_t *self) {
    shutdown(self->fd, SHUT_RDWR);
    close(self->fd);
}

int socket_bind_and_listen(socket_t *self, struct addrinfo *ptr) {
    int s = bind(self->fd, ptr->ai_addr, ptr->ai_addrlen);
    if (s == -1) {
        printf("Error: %s\n", strerror(errno));
        close(self->fd);
        freeaddrinfo(ptr);
        return 1;
    }
    s = listen(self->fd, PEERS_WAITING_MAX);
    if (s == -1) {
        printf("Error: %s\n", strerror(errno));
        close(self->fd);
        return 1;
    }
    return 0;  
}
int socket_accept(socket_t *listener, socket_t *peer) {
    peer->fd = accept(listener->fd, NULL, NULL);
    if (peer->fd == -1) {
        printf("Error: %s\n", strerror(errno));
        return 1;
    }
    return 0;
}

int socket_connect(socket_t *self, struct addrinfo *ptr) {
    int s = connect(self->fd, ptr->ai_addr, ptr->ai_addrlen);
    if (s == -1) {
        printf("Error: %s\n", strerror(errno));
        close(self->fd);
    }
    return 0;
}

ssize_t socket_send(socket_t *self, const char *buffer, size_t length) {
    ssize_t s = send(self->fd, buffer, length, 0);
    return s;
}
ssize_t socket_receive(socket_t *self, char *buffer, size_t length) {
    ssize_t s = recv(self->fd, buffer, length, 0);
    if (s == -1) {
        printf("Error: %s\n", strerror(errno));
        close(self->fd);
        return -1;
    }
    if (s == 0) {
        close(self->fd);
        return 1;
    }
    return 0;  
}
