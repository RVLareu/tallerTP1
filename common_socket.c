#include <stdbool.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>

#include "socket.h"

void socket_init(socket_t *self) {
    self->fd = socket(AF_INET, SOCK_STREAM, 0);
    /*    if (skt == -1) {
        printf("Error: %s\n", strerror(errno));
        freeaddrinfo(ptr);
        return 1;
    }*/
}

void socket_uninit(socket_t *self) {
    shutdown(self->fd, SHUT_RDWR);
    close(self->fd);
}

void socket_bind_and_listen(socket_t *self, const char *host, const char *service) {
    struct addrinfo hints, *ptr;
    int e = 0;
    int val = 1;
    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_INET;        
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
        //return 1
    }

    e = bind(self->fd, ptr->ai_addr, ptr->ai_addrlen);
    if (e == -1) {
        printf("Error: %s\n", strerror(errno));
        close(self->fd);
        freeaddrinfo(ptr);
        //return 1
    }   
    freeaddrinfo(ptr);

    e = listen(self->fd, 8);
    if (e == -1) {
        printf("Error: %s\n", strerror(errno));
        close(self->fd);
        //return 1
    }
}


void socket_accept(socket_t *listener, socket_t *peer) {
    peer->fd = accept(listener->fd, NULL, NULL);
    if (peer->fd == -1) {
        printf("Error: %s\n", strerror(errno));
        //continue_running = false
        //accept socket_valid = false
    }
}

void socket_connect(socket_t *self, const char *host, const char *service) {
    int e = 0;
    struct addrinfo hints;
    struct addrinfo *result, *ptr;
    bool are_we_connected = false;


    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_INET;       
    hints.ai_socktype = SOCK_STREAM; 
    hints.ai_flags = 0;

    e = getaddrinfo(host, service, &hints, &result);   
    if (e != 0) { 
        printf("Error in getaddrinfo: %s\n", gai_strerror(e));
        //return 1;
    }

    for (ptr = result; ptr != NULL && are_we_connected == false; ptr = ptr->ai_next) {
        self->fd = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
        if (self->fd == -1) {
            printf("Error: %s\n", strerror(errno));
        } else {
            e = connect(self->fd, ptr->ai_addr, ptr->ai_addrlen);
            if (e == -1) {
                printf("Error: %s\n", strerror(errno));
                close(self->fd);
            }
            are_we_connected = (e != -1);
            if (are_we_connected == false) {
                //return 1;
            }
        }
    }
    freeaddrinfo(result);

}

ssize_t socket_send(socket_t *self, const char *buffer, size_t length){
    int sent = 0;
    int s = 0;
    bool is_the_socket_valid = true;

    while (sent < length && is_the_socket_valid) {
        s = send(self->fd, &buffer[sent], sizeof(char), MSG_NOSIGNAL);

        if (s == 0) {
            is_the_socket_valid = false;
        }
        else if (s == -1) {
            is_the_socket_valid = false;
        }
        else {
            sent += s;
        }
    }

    if (is_the_socket_valid) {
        return sent;
    }
    else {
        return -1;
    }
}
ssize_t socket_receive(socket_t *self, char *buffer, size_t length){
    int received = 0;
    int s = 0;
    bool is_the_socket_valid = true;

    while (received < length && is_the_socket_valid) {
        s = recv(self->fd, &buffer[received], sizeof(char), 0);

        if (s == 0) { // nos cerraron el socket :(
            is_the_socket_valid = false;
        }
        else if (s == -1) { // hubo un error >(
            is_the_socket_valid = false;
        }
        else {
            received += s;
        }
    }

    if (is_the_socket_valid) {
        return received;
    }
    else {
        return -1;
    }
}



int recv_message(int skt, char *buf, int size) {
    int received = 0;
    int s = 0;
    bool is_the_socket_valid = true;

    while (received < size && is_the_socket_valid) {
        s = recv(skt, &buf[received], sizeof(char), 0);

        if (s == 0) { // nos cerraron el socket :(
            is_the_socket_valid = false;
        }
        else if (s == -1) { // hubo un error >(
            is_the_socket_valid = false;
        }
        else {
            received += s;
        }
    }

    if (is_the_socket_valid) {
        return received;
    }
    else {
        return -1;
    }
}


int send_message(int skt, char *buf, int size) {
    int sent = 0;
    int s = 0;
    bool is_the_socket_valid = true;

    while (sent < size && is_the_socket_valid) {
        s = send(skt, &buf[sent], sizeof(char), MSG_NOSIGNAL);

        if (s == 0) {
            is_the_socket_valid = false;
        }
        else if (s == -1) {
            is_the_socket_valid = false;
        }
        else {
            sent += s;
        }
    }

    if (is_the_socket_valid) {
        return sent;
    }
    else {
        return -1;
    }
}




