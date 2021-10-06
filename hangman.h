#include <stdio.h>
#include "game.h"
#include "lineReader.h"

typedef struct {
    int victories;
    int defeats;
    int positionInArray;
    game_t games[1000]; // recorrer file hasta final contando lineas
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
int hangman_getActualGameID(hangman_t *self);
int hangman_getDefeats(hangman_t * self);
int hangman_getVictories(hangman_t * self);
