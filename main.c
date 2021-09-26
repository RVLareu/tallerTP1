#include <stdio.h>
#include <stdlib.h>
#include "hangman.h"

int main(int argc, char *argv[]) {

    FILE *stream;
    int attemps;

    if (argc > 1) {
        stream = fopen(argv[1], "r");
        attemps = atoi(argv[2]);
    } else {
        stream = stdin;
    }

    /////Creacion de Hangman/////
    hangman_t hangman;
    int hangmanCreationError = hangman_init(&hangman, stream);

    //Creo juego
    int gameCreationError = hangman_createGame(&hangman, attemps);

    while (1) {
        char letter;
        printf("Palabra secreta: %s\n", hangman_getDisplayWord(&hangman, 0));
        printf("Te quedan %d intentos:\n", hangman_gameAttempsLeft(&hangman, 0));
        printf("Ingrese letra: ");
        scanf("%s", &letter);
        int guess = hangman_guessLetter(&hangman, 0, letter);

        if (guess == 2)
        {
            char *secretWord = hangman_getGameWord(&hangman, 0);

            printf("Perdiste! La palabra secreta era: '%s'", secretWord);
            break;
        }
        else if (guess == 3)
        {
            printf("Ganaste!!\n");
            break;
        }
    }

    ///// DESTRUO HANGMAN//////
    hangman_uninit(&hangman);

    ///// CIERRO ARCHIVO ///////
    fclose(stream);
}