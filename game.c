#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "game.h"

#define END_GAME_LOSS 2
#define END_GAME_WIN 3

int game_init(game_t *self, char *word, int attemps, int id) {
    strtok(word, "\n");
    self->word = word;
    self->attemps = attemps;
    self->id = id;
    int lenWord = strlen(word);
    //ESTO HACE QUILOMBO EN VALGRIND
    self->displayWord = (char *)malloc(sizeof(char)*lenWord);
    for (int i = 0; i < lenWord; i++ ) {
        self->displayWord[i] = '_';
    }
    printf("WORD:%s", word);
    return 0;
}

int game_uninit(game_t *self) {
    free(self->displayWord);
    return 0;
}

int game_guessLetter(game_t *self, char letter) {
    int present = 0;
    for(int i = 0; i < strlen(self->word); i++) {
        if (self->word[i] == letter) {
            
            present = 1;
  
            self->displayWord[i] = letter;
        }
    }

    if (present == 0) {
        self->attemps --;
        if (self->attemps == 0) {
            return END_GAME_LOSS;
        }
    }
    if (present == 1 && strchr(self->displayWord, '_') == NULL) {
        return END_GAME_WIN;
    }
    return present;
}


char *game_getWord(game_t *self) {
    return self->word;
}

int game_getAttempsLeft(game_t *self) {
    return self->attemps;
}

char *game_getDisplayWord(game_t *self) {
    return self->displayWord;
}