#include <stdbool.h>

#include "socket.h"
#include "hangman.h"
/*server dedicated to run a hangman
    -socket: server socket
    -hangman: hangman game
*/
typedef struct {
    socket_t socket;
    hangman_t hangman;
} server_t;
/*initiates a server*/
void server_init(server_t *self, const char *servicename, FILE *stream);
/*closes/destroys server*/
void server_uninit(server_t *self);
/*creates a game (inside hangman)*/
int server_create_game(server_t *self, int attemps_for_client);
/*accepts incoming client*/
void server_accept_client(server_t *self, socket_t *socket_client);
/*creates message with information for player*/
char* server_create_message_len(server_t *self, bool end_game, char *buffer);
/*guesses a letter of a game*/
bool server_guess_hangman_letter(server_t *self, char* buffer);
/*prints recap of games played*/
void server_print_recap(server_t *self);
/*send a message*/
int server_send_message(server_t *self,
                             socket_t *peerskt,
                                 char* buffer,
                                  size_t lenght);
/*receives a message*/
int server_receive_message(server_t *self,
                             socket_t *peerskt,
                                 char* buffer,
                                     size_t lenght);
