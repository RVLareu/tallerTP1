#include <stdio.h>
#include <string.h>
#include "hangman.h"



/*
int main(int argc, char* argv[]) {
    hangman_t hangman;
    hangman_init(&hangman, fopen(argv[1], "r"));
    hangman_createGame(&hangman,5);
    printf("%s\n",hangman_getDisplayWord(&hangman,0));
    hangman_guessLetter(&hangman,0,'c');
    printf("%s\n",hangman_getDisplayWord(&hangman,0));
    hangman_guessLetter(&hangman,0,'z');
    printf("%s\n",hangman_getDisplayWord(&hangman,0));
    hangman_guessLetter(&hangman,0,'s');
    printf("%s\n",hangman_getDisplayWord(&hangman,0));
    hangman_guessLetter(&hangman,0,'a');
    printf("%s\n",hangman_getDisplayWord(&hangman,0));

    hangman_createGame(&hangman,5);
    printf("%s\n",hangman_getDisplayWord(&hangman,1));
    hangman_guessLetter(&hangman,1,'h');
    printf("%s\n",hangman_getDisplayWord(&hangman,1));
    hangman_guessLetter(&hangman,1,'o');
    printf("%s\n",hangman_getDisplayWord(&hangman,1));
    hangman_guessLetter(&hangman,1,'s');
    printf("%s\n",hangman_getDisplayWord(&hangman,1));
    hangman_guessLetter(&hangman,1,'a');
    printf("%s\n",hangman_getDisplayWord(&hangman,1));
    

    hangman_uninit(&hangman);

}

*/
int hangman_init(hangman_t *self, FILE *stream) {
    self->victories = 0;
    self->defeats = 0;
    self->positionInArray = 0;
    self->stream = stream;
    lineReader_init(&self->lineReader, stream);
    return 0;
}

int hangman_uninit(hangman_t *self) {
    for (int i = 0; i < self->positionInArray; i++) {
        printf("ENTRO");
        game_uninit(&self->games[i]);
    }
    lineReader_uninit(&self->lineReader);
    return 0;
}

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

