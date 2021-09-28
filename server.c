#include <sys/types.h>                                                                                                     
#include <sys/socket.h>                                                                                                     
#include <netdb.h>                                                                                                         
#include <string.h>                                                                                                         
#include <stdio.h>                                                                                                         
#include <stdlib.h>                                                                                                         
#include <unistd.h>
#include <errno.h>                                                                                                         
#include <stdbool.h>

#include "hangman.h"
#include "socket.h"

int main(int argc, char* argv[]) {
    
    /*------VARIABLES SOCKET--------*/
    int s = 0, skt, peerskt;                                                                                                                                                                                                                                                                      
    int val = 1;
    struct addrinfo hints, *ptr;
    bool continue_running = true;
    bool is_the_accept_socket_valid = true;  

    /*------VARIABLES SCOKET--------*/
    FILE* stream = fopen(argv[2], "r");
    int attemps = atoi(argv[3]);

    /*------GETADDRINFO--------*/
    memset(&hints, 0, sizeof(struct addrinfo));  
    hints.ai_family = AF_INET;                                                                                            
    hints.ai_socktype = SOCK_STREAM;                                                                                    
    hints.ai_flags = AI_PASSIVE;          

    s = getaddrinfo(NULL, argv[1], &hints, &ptr);                                                   
    if (s != 0) {
      printf("Error in getaddrinfo: %s\n", gai_strerror(s));
      return 1;
    }

    /*------SOCKET INIT BIND LISTEN--------*/
    socket_t socket;
    skt = socket_init(&socket, ptr);
    socket_bind_and_listen(&socket, ptr);                                           

    freeaddrinfo(ptr);
    printf("LISTENING... en port: %s\n", argv[1]);
    

    /*------HANGMAN--------*/
    hangman_t hangman;
    int hangmanCreationError = hangman_init(&hangman, stream);
    



    /*------ACCEPTING--------*/
    while (continue_running) {

        /*------ACEPTO AL CLIENTE--------*/
        socket_t peersocket;
        int peerskt = socket_accept(&socket, &peersocket);                                                                                   
        if (peerskt == -1) {
            continue_running = false;
            is_the_accept_socket_valid = false;
        }
        /*------INICIO JUEGO--------*/
        int gameCreationError = hangman_createGame(&hangman, attemps);
        if (gameCreationError == -1) {
            continue_running = false;
            continue;
        }
        int gameID = hangman_getActualGameID(&hangman);

        /*------PRIMER MENSAJE AL CLIENTE CON JUEGO--------*/
        
        char* display_word = hangman_getDisplayWord(&hangman, gameID);
        int attemps_left = hangman_gameAttempsLeft(&hangman, gameID);
        unsigned char attempsLeftBinary;
        attempsLeftBinary = (unsigned char)attemps_left;
        
        short wordLen = (short)strlen(display_word);
        wordLen = wordLen | 0x0000;
        
        char sendBuff[sizeof(char)*wordLen + sizeof(unsigned char) + sizeof(char)];
        snprintf(sendBuff, sizeof(sendBuff), "%u%hi%s", attemps_left, wordLen , display_word);
        printf("SEND: %s", sendBuff);
        s = socket_send(&peersocket, sendBuff, sizeof(sendBuff));
        if (s == -1) {
            printf("Error: %s\n", strerror(errno));
            return 1;
        }


        /*------JUEGO EN DESARROLLO--------*/
        while(1) {
            /*------RECIBO PALABRA DEL CLIENTE--------*/
            char buffer[1];
            s = socket_receive(&peersocket, buffer, sizeof(buffer));
            if (s == -1) {
                return 1;
            } 
            if (s == 1) { 
                break;
            }

            /*------ADIVINO EN HANGMAN CON LETRA DEL CLIENTE--------*/
            int guess = hangman_guessLetter(&hangman, gameID, buffer[0]);
            char* display_word = hangman_getDisplayWord(&hangman, gameID);
            int attemps_left = hangman_gameAttempsLeft(&hangman, gameID);
            int wordLen = strlen(display_word);
            char sendBuff[sizeof(display_word) + sizeof(attemps_left) + sizeof(wordLen)];
            snprintf(sendBuff, sizeof(sendBuff), "%d%d%s", attemps_left, wordLen , display_word);

            
            /*------ENVIO ESTADO ACTUALIZADO CON ADIVINANZA DEL CLIENTE--------*/
            s = socket_send(&peersocket, sendBuff, sizeof(sendBuff));
            if (s == -1) {
                printf("Error: %s\n", strerror(errno));
                return 1;           
            }
        }
    }       

    /*------FIN--------*/
    socket_uninit(&socket);

    if (!is_the_accept_socket_valid) {
        return 1;
    } else {
        printf("Resumen:\n");
        printf("\tVictorias: %d\n", hangman_getVictories(&hangman));
        printf("\tDerrotas: %d\n", hangman_getDefeats(&hangman));
        hangman_uninit(&hangman);
        fclose(stream);
        return 0;
    }
}



