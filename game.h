#include <stdio.h>
/*  -word: complete word to guess
    -displayWord: word with '_' where the letter has not been guessed yet
    -attemps: number of bad guesses before loosing
    -id: unique identifier of the game
*/
typedef struct {
    char* word;
    char* displayWord;
    int attemps;
    int id;
} game_t;

/*initiates a game*/
int game_init(game_t *self, char *word, int attemps, int id);
/*unitiates game*/
int game_uninit(game_t *self);
/*guess letter of game*/
int game_guessLetter(game_t *self, char letter);
/*returns word of game*/
char *game_getWord(game_t *self);
/*returns the word of the game*/
int* game_getGuessed(game_t *self);
/*returns attemps left*/
int game_getAttempsLeft(game_t *self);
/*returns the word to diplay*/
char *game_getDisplayWord(game_t *self);
