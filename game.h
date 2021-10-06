#include <stdio.h>

typedef struct {
    char* word;
    char* displayWord;
    int attemps;
    int id;
} game_t;


int game_init(game_t *self, char *word, int attemps, int id);

int game_uninit(game_t *self);

int game_guessLetter(game_t *self, char letter);

char *game_getWord(game_t *self);

int* game_getGuessed(game_t *self);

int game_getAttempsLeft(game_t *self);

char *game_getDisplayWord(game_t *self);
