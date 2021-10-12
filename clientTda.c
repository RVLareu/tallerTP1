#include "clientTda.h"

/*initiates client and connects it to server*/
void client_init_connect(client_t *self,
                     const char *hostname,
                         const char *servicename) {
    socket_connect(&self->socket, hostname, servicename);
}
/*destroys client*/
void client_uninit(client_t *self) {
    socket_uninit(&self->socket);
}
/*receives a message from server, process it and
prints result from it.
*/
int client_receive_process(client_t *self,
                             char* buffer,
                                 size_t size,
                                     int ingame) {
    socket_receive(&self->socket, buffer, size);
    int attemps = (int) buffer[0] & 0x7F;
    int state = (int)buffer[0] & 0x80;
    unsigned short len = 0;
    memcpy(&len, &buffer[1], 2);
    len = ntohs(len);
    char *rbuffer = calloc(sizeof(char), len+1);
    socket_receive(&self->socket, rbuffer, len);

    if (state == 128) {
        if (attemps == 0) {
            printf("\nPerdiste! La palabra secreta era: '%s'", rbuffer);
        } else {
            printf("\nGanaste!!");
        }
        free(rbuffer);
        return 1;
    }
    if (ingame) {
        printf("\nPalabra secreta: %s\n", rbuffer);
    } else {
        printf("Palabra secreta: %s\n", rbuffer);
    }
    printf("Te quedan %d intentos\n", attemps);
    printf("Ingrese letra: ");
    free(rbuffer);
    return 0;
}


/*sends a message*/
int client_send(client_t *self, char *letter, int size) {
    int e = socket_send(&self->socket, letter, size);
    return e;
}


