#include <stdio.h>
#include <string.h>
#include "hangman.h"


int hangman_init(hangman_t *self, FILE *stream) {
    self->victorias = 0;
    self->derrotas = 0;
    self->positionInArray = 0;
    self->stream = stream;
    lineReader_init(&self->lineReader, stream);
    return 0;
}

int hangman_uninit(hangman_t *self) {
    lineReader_uninit(&self->lineReader);
    for (int i = 0; i < self->positionInArray; i++) {
        game_t game;
        game_uninit(&self->games[i]);
    }
    return 0;
}

int hangman_createGame(hangman_t *self, int attemps) {
    game_t game;

    lineReader_readLine(&self->lineReader);
    char* line = lineReader_sendLine(&self->lineReader);
    game_init(&game, line, attemps, self->positionInArray);

    self->games[self->positionInArray] = game;
    self->positionInArray = self->positionInArray + 1;
    return 0;
}

int hangman_guessLetter(hangman_t *self, int gameID, char letter) {
    
    int guess = game_guessLetter(&self->games[gameID], letter);
    if (guess == 2) {
        self->victorias ++;
    }
    if (guess == 3) {
        self->derrotas ++;
    }
    
    return guess;
}

char* hangman_getDisplayWord(hangman_t *self, int gameID) {
    return game_getDisplayWord(&self->games[gameID]);
}

int hangman_gameAttempsLeft(hangman_t *self, int gameID) {
    return game_getAttempsLeft(&self->games[gameID]);
}

char *hangman_getGameWord(hangman_t *self, int gameID) {
    return game_getWord(&self->games[gameID]);
}