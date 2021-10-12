#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "game.h"

#define END_GAME_LOSS 2
#define END_GAME_WIN 3

/*receives a word, number of attemps and an id. Creates a game*/
int game_init(game_t *self, char *word, int attemps, int id) {
    self->word = word;
    self->attemps = attemps;
    self->id = id;
    int lenWord = strlen(word);
    self->displayWord = calloc(sizeof(char), lenWord + 1);
    for (int i = 0; i < lenWord; i++) {
        self->displayWord[i] = '_';
    }
    return 0;
}
/*destroys game, freeing the line used to display a word*/
int game_uninit(game_t *self) {
    free(self->displayWord);
    return 0;
}
/*receives a letter and returns:
    -0 if wrong guess
    -2 if end game with loss
    -3 if end game with win    
*/
int game_guessLetter(game_t *self, char letter) {
    int present = 0;
    for (int i = 0; i < strlen(self->word); i++) {
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
    }else if (present == 1 && strchr(self->displayWord, '_') == NULL) {
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
/*returns the word to display, wich has '_' when the letter has not been guessed yet*/
char *game_getDisplayWord(game_t *self) {
    return self->displayWord;
}
