#include <string.h>
#include <arpa/inet.h>

#include "serverTda.h"

/*initiates a server, with its socket annd its hangman*/
void server_init(server_t *self, const char *servicename, FILE *stream) {
    socket_init(&self->socket);
    socket_bind_and_listen(&self->socket, NULL, servicename);
    hangman_init(&self->hangman, stream);
}
/*destroys socket and hangman*/
void server_uninit(server_t *self) {
    hangman_uninit(&self->hangman);
    socket_uninit(&self->socket);
}
/*creates a game*/
int server_create_game(server_t *self, int attemps_for_client) {
    return hangman_createGame(&self->hangman, attemps_for_client);
}

void server_accept_client(server_t *self, socket_t *socket_client) {
    socket_accept(&self->socket, socket_client);
}
/*creates a message to send to the client.
receives a buffer where it put its first 3 bytes of message and
returns display_word, correspondig to the rest
of the message
*/
char* server_create_message_len(server_t *self, bool end_game, char *buffer) {
    int game_id = hangman_getActualGameID(&self->hangman);
    char* display_word = hangman_getDisplayWord(&self->hangman, game_id);
    unsigned short len_word = strlen(display_word);
    len_word = htons(len_word);
    memcpy(&buffer[1], &len_word, 2);
    unsigned char attemps = hangman_gameAttempsLeft(&self->hangman, game_id);
    if (end_game) {
        attemps = attemps | 0x80;
        display_word = hangman_getGameWord(&self->hangman, game_id);
    }
    memcpy(&buffer[0], &attemps, 1);
    return display_word;
}
/*guesse a letter returning if game has ended or not*/
bool server_guess_hangman_letter(server_t *self, char* buffer) {
    int game_id = hangman_getActualGameID(&self->hangman);
    int guess =  hangman_guessLetter(&self->hangman, game_id, buffer[0]);
    if (guess == 2 || guess == 3) {
        return true;
    }
    return false;
}
/*prints recap of victories and defeats*/
void server_print_recap(server_t *self) {
    printf("Resumen:\n");
    printf("\tVictorias: %d\n",hangman_getVictories(&self->hangman));
    printf("\tDerrotas: %d\n",hangman_getDefeats(&self->hangman));
}

/*sends message*/
int server_send_message(server_t *self,
                         socket_t *peerskt,
                             char* buffer,
                                 size_t lenght) {
    return socket_send(peerskt, buffer, lenght);
}
/*receives a message*/
int server_receive_message(server_t *self,
                             socket_t *peerskt,
                                 char* buffer,
                                     size_t lenght) {
    return socket_receive(peerskt, buffer, lenght);
}
