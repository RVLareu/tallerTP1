#include <stdbool.h>
#include <sys/types.h>
#include <sys/socket.h>

#include "socket.h"

void socket_init(socket_t *self) {
    
}
void socket_uninit(socket_t *self) {

}

void socket_bind_and_listen(socket_t *self, const char *host, const char *service) {
    
}

void socket_accept(socket_t *listener, socket_t *peer){}

void socket_connect(socket_t *self, const char *host, const char *service){}

ssize_t socket_send(socket_t *self, const char *buffer, size_t length){
    return 0;
}
ssize_t socket_receive(socket_t *self, char *buffer, size_t length){
    return 0;
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




