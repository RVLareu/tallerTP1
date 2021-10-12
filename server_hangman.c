#include <stdio.h>
#include <string.h>
#include "hangman.h"

/*initiates a hangman with its lineReader and stream*/
int hangman_init(hangman_t *self, FILE *stream) {
    self->victories = 0;
    self->defeats = 0;
    self->positionInArray = 0;
    self->stream = stream;
    lineReader_init(&self->lineReader, stream);
    return 0;
}
/*destroys hangman, inlcluding lineReader and games y games array*/
int hangman_uninit(hangman_t *self) {
    for (int i = 0; i < self->positionInArray; i++) {
        game_uninit(&self->games[i]);
    }
    lineReader_uninit(&self->lineReader);
    return 0;
}
/*creates a game and stores it in games arrays. Updates positionInArray*/
int hangman_createGame(hangman_t *self, int attemps) {
    int err = lineReader_readLine(&self->lineReader);
    if (err == -1) {
        return -1;
    } else {
        game_t game;
        char* line = lineReader_sendLine(&self->lineReader);
        game_init(&game, line, attemps, self->positionInArray);
        self->games[self->positionInArray] = game;
        self->positionInArray = self->positionInArray + 1;
        return 0;
    }
}
/*guesses a letter from a game, updating defeats or victories if necessary*/
int hangman_guessLetter(hangman_t *self, int gameID, char letter) {
    int guess = game_guessLetter(&self->games[gameID], letter);
    if (guess == 2) {
        self->defeats ++;
    } else if (guess == 3) {
        self->victories ++;
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

int hangman_getActualGameID(hangman_t *self) {
    return self->positionInArray - 1;
}

int hangman_getDefeats(hangman_t * self) {
    return self->defeats;
}

int hangman_getVictories(hangman_t * self) {
    return self->victories;
}

