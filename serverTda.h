#include <stdbool.h>

#include "socket.h"
#include "hangman.h"

typedef struct {
    socket_t socket;
    hangman_t hangman;
} server_t;

void server_init(server_t *self, const char *servicename, FILE *stream);

void server_uninit(server_t *self);

int server_create_game(server_t *self, int attemps_for_client);

void server_accept_client(server_t *self, socket_t *socket_client);

char* server_create_message_len(server_t *self, bool end_game, char *buffer);

bool server_guess_hangman_letter(server_t *self, char* buffer);

void server_print_recap(server_t *self);

int server_send_message(server_t *self, socket_t *peerskt, char* buffer, size_t lenght);

int server_receive_message(server_t *self, socket_t *peerskt, char* buffer, size_t lenght);

