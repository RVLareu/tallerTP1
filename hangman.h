#include <stdio.h>
#include "game.h"
#include "lineReader.h"

typedef struct {

    int victorias;
    int derrotas;
    int positionInArray;
    game_t games[1000];
    FILE* stream;
    lineReader_t lineReader;
     
} hangman_t;



int hangman_init(hangman_t *self, FILE *stream);
int hangman_uninit(hangman_t *self);

int hangman_createGame(hangman_t *self, int attemps); 

int hangman_guessLetter(hangman_t *self, int gameID, char letter);

char* hangman_getDisplayWord(hangman_t *self, int gameID);
int hangman_gameAttempsLeft(hangman_t *self, int gameID);
char *hangman_getGameWord(hangman_t *self, int gameID);