#include <stdbool.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>

#include "socket.h"
/*creates socket with AF_INET, SOCK_STREAM*/
void socket_init(socket_t *self) {
    self->fd = socket(AF_INET, SOCK_STREAM, 0);
    if (self->fd == -1) {
        printf("Error: %s\n", strerror(errno));
        close(self->fd);
    }
}
/*destroys socket*/
void socket_uninit(socket_t *self) {
    shutdown(self->fd, SHUT_RDWR);
    close(self->fd);
}
/*binds socket and is set to listen up to 8. Uses
getaddrinfo, setsockopt(), bind() and liste()
*/
void socket_bind_and_listen(socket_t *self,
                             const char *host,
                                 const char *service) {
    struct addrinfo hints, *ptr;
    int e = 0;
    int val = 1;
    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_UNSPEC;        
    hints.ai_socktype = SOCK_STREAM; 
    hints.ai_flags = AI_PASSIVE;
    e = getaddrinfo(host, service, &hints, &ptr);
    if (e != 0) {
        printf("Error in getaddrinfo: %s\n", gai_strerror(e));
    }

    
    e = setsockopt(self->fd, SOL_SOCKET, SO_REUSEADDR, &val, sizeof(val));
    if (e == -1) {
        printf("Error: %s\n", strerror(errno));
        close(self->fd);
        freeaddrinfo(ptr);
    }

    e = bind(self->fd, ptr->ai_addr, ptr->ai_addrlen);
    if (e == -1) {
        printf("Error: %s\n", strerror(errno));
        close(self->fd);
        freeaddrinfo(ptr);
    }   
    freeaddrinfo(ptr);

    e = listen(self->fd, 8);
    if (e == -1) {
        printf("Error: %s\n", strerror(errno));
        close(self->fd);
    }
}

/*accepts an incomming socket (peer)*/
void socket_accept(socket_t *listener, socket_t *peer) {
    // duda de si rompe encapsulamiento
    peer->fd = accept(listener->fd, NULL, NULL);
    if (peer->fd == -1) {
        printf("Error: %s\n", strerror(errno));
        close(peer->fd);
    }
}
/*connects to socket (from server). Uses getaddrinfo, socket()
and connect()
*/
void socket_connect(socket_t *self, const char *host, const char *service) {
    int e = 0;
    struct addrinfo hints;
    struct addrinfo *result, *ptr;
    bool connected = false;

    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_UNSPEC;       
    hints.ai_socktype = SOCK_STREAM; 
    hints.ai_flags = 0;

    e = getaddrinfo(host, service, &hints, &result);   
    if (e != 0) { 
        printf("Error in getaddrinfo: %s\n", gai_strerror(e));
        //return 1;
    }

    for (ptr = result; ptr != NULL && connected == false; ptr = ptr->ai_next) {
        self->fd = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
        if (self->fd == -1) {
            printf("Error: %s\n", strerror(errno));
            close(self->fd);
        } else {
            e = connect(self->fd, ptr->ai_addr, ptr->ai_addrlen);
            if (e == -1) {
                printf("Error: %s\n", strerror(errno));
                close(self->fd);
            }
            connected = (e != -1);
            if (connected == false) {
                printf("Error: %s\n", strerror(errno));
                close(self->fd);
            }
        }
    }
    freeaddrinfo(result);
}

/*sends a buffer char by char. Taken and modified from 
github.com/Taller-de-Programacion/clases/tree/master/sockets-mdipaola
*/
ssize_t socket_send(socket_t *self, const char *buffer, size_t length){
    int sent = 0;
    bool is_the_socket_valid = true;
    while (sent < length && is_the_socket_valid) {
        int s = 0;
        s = send(self->fd, &buffer[sent], sizeof(char), MSG_NOSIGNAL);
        if (s == 0) {
            is_the_socket_valid = false;
        } else if (s == -1) {
            is_the_socket_valid = false;
        } else {
            sent += s;
        }
    }
    if (is_the_socket_valid) {
        return sent;
    } else {
        return -1;
    }
}
/*receives a buffer char by char. Taken and modified from 
github.com/Taller-de-Programacion/clases/tree/master/sockets-mdipaola
*/
ssize_t socket_receive(socket_t *self, char *buffer, size_t length){
    int received = 0;
    bool is_the_socket_valid = true;
    while (received < length && is_the_socket_valid) {
        int s = 0;
        s = recv(self->fd, &buffer[received], sizeof(char), 0);
        if (s == 0) { 
            is_the_socket_valid = false;
        } else if (s == -1) { 
            is_the_socket_valid = false;
        } else {
            received += s;
        }
    }
    if (is_the_socket_valid) {
        return received;
    } else {
        return -1;
    }
}

