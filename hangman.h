#include <stdio.h>
#include "game.h"
#include "lineReader.h"
/*
    -victories: number of current game wins
    -defeats: number of current game losses
    -positionInArray: current position in array of games
    -games: array of games
    -stream: file with words
    -lineReader: to read from file
*/
typedef struct {
    int victories;
    int defeats;
    int positionInArray;
    game_t games[1000]; // recorrer file hasta final contando lineas
    FILE* stream;
    lineReader_t lineReader;
} hangman_t;


/*creates hangman*/
int hangman_init(hangman_t *self, FILE *stream);
/*destroys hangman*/
int hangman_uninit(hangman_t *self);
/*creates a game*/
int hangman_createGame(hangman_t *self, int attemps); 
/*guesses a letter from a game*/
int hangman_guessLetter(hangman_t *self, int gameID, char letter);
/*returns a display word from a game*/
char* hangman_getDisplayWord(hangman_t *self, int gameID);
/*return attemps left from a game*/
int hangman_gameAttempsLeft(hangman_t *self, int gameID);
/*returns word from a game*/
char *hangman_getGameWord(hangman_t *self, int gameID);
/*returns current game id*/
int hangman_getActualGameID(hangman_t *self);
/*return current amount of defeats*/
int hangman_getDefeats(hangman_t * self);
/*return current amount of victories*/
int hangman_getVictories(hangman_t * self);
