
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>

#include "socket.h"
#include "hangman.h"


int main(int argc, char* argv[]) {
    /*---VARIABLES---*/
    int e = 0;
    struct addrinfo hints, *ptr;
    const char *servicename = argv[1];
    bool continue_running = true;
    bool is_the_accept_socket_valid = true;
    FILE *stream = fopen(argv[3], "r");
    int attemps_for_client = atoi(argv[2]);
    
    /*---GETADDRINFO---*/
    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_INET;        
    hints.ai_socktype = SOCK_STREAM; 
    hints.ai_flags = AI_PASSIVE;
    e = getaddrinfo(NULL, servicename, &hints, &ptr);
    if (e != 0) {
        printf("Error in getaddrinfo: %s\n", gai_strerror(e));
        return 1;
    }
    /*---SOCKET---*/
    int skt = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
    if (skt == -1) {
        printf("Error: %s\n", strerror(errno));
        freeaddrinfo(ptr);
        return 1;
    }
    /*---REUSAR---*/
    int val = 1;
    e = setsockopt(skt, SOL_SOCKET, SO_REUSEADDR, &val, sizeof(val));
    if (e == -1) {
        printf("Error: %s\n", strerror(errno));
        close(skt);
        freeaddrinfo(ptr);
        return 1;
    }
    /*---BIND---*/
    e = bind(skt, ptr->ai_addr, ptr->ai_addrlen);
    if (e == -1) {
        printf("Error: %s\n", strerror(errno));
        close(skt);
        freeaddrinfo(ptr);
        return 1;
    }

    /*---FREE ADDRINFO---*/
    freeaddrinfo(ptr);

    /*---LISTEN---*/
    e = listen(skt, 8);
    if (e == -1) {
        printf("Error: %s\n", strerror(errno));
        close(skt);
        return 1;
    }

    /*---HANGMAN---*/
    hangman_t hangman;
    hangman_init(&hangman, stream);

    /*---ACCEPTING LOOP---*/
    while (continue_running) {
        e = hangman_createGame(&hangman, attemps_for_client);
        if (e == -1) {
            break;
        }
        int peerskt = accept(skt, NULL, NULL);

        if (peerskt == -1) {
            printf("Error: %s\n", strerror(errno));
            continue_running = false;
            is_the_accept_socket_valid = false;
        } else {
            
            bool end_game = false;
            while (1) {
                
                int game_id = hangman_getActualGameID(&hangman);
                char* display_word = hangman_getDisplayWord(&hangman, game_id);

                unsigned short len_word = strlen(display_word);
                
                char len[3];
                len_word = htons(len_word);
                memcpy(&len[1], &len_word, 2);

                unsigned char attemps = hangman_gameAttempsLeft(&hangman, game_id);
                if (end_game) {
                    attemps = attemps | 0x80;
                    display_word = hangman_getGameWord(&hangman, game_id);
                }
                memcpy(&len[0], &attemps, 1);

                /*---SEND---*/
                
                e = send_message(peerskt, len, 3);
                if (e == -1) {
                    printf("Error: %s\n", strerror(errno));
                    break;
                }
                e = send_message(peerskt, display_word, strlen(display_word));
                if (e == -1) {
                    printf("Error: %s\n", strerror(errno));
                    break;
                }
                if (end_game) break;

                /*---RECEIVE---*/

                char rec_buff[1];
                e = recv_message(peerskt, rec_buff, 1);
                if (e == -1) {
                    printf("Error: %s\n", strerror(errno));
                    break;
                }

                int guess = hangman_guessLetter(&hangman, game_id, rec_buff[0]);
                if (guess == 2 || guess == 3) {
                    end_game = true;
                }
            }

        }
        
              
    }
    printf("Resumen:\n");
    printf("\tVictorias: %d\n",hangman_getVictories(&hangman));
    printf("\tDerrotas: %d\n",hangman_getDefeats(&hangman));
    hangman_uninit(&hangman);
    shutdown(skt, SHUT_RDWR);
    close(skt);
    /*---FINISHING---*/
    if (is_the_accept_socket_valid) {
        return 1;
    }
    else {
        return 0;
    }
}
