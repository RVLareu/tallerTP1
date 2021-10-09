
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
#include "serverTda.h"

int main(int argc, char* argv[]) {
    const char *servicename = argv[1];
    int attemps_for_client = atoi(argv[2]);
    FILE *stream = fopen(argv[3], "r");
    server_t server;
    server_init(&server, servicename, stream);
    socket_t peerskt;
    while (1) {
        int e = 0;
        e = server_create_game(&server, attemps_for_client);
        if (e == -1) {
            break;
        }
        
        server_accept_client(&server, &peerskt);
        bool end_game = false;
        
        char len_fm[3];
        char* wo_fm = server_create_message_len(&server, end_game, len_fm);
        e = server_send_message(&server, &peerskt, len_fm, 3);
        if (e == -1) {
            printf("Error: %s\n", strerror(errno));
            break;
        }
        e = server_send_message(&server, &peerskt, wo_fm, strlen(wo_fm));
        if (e == -1) {
            printf("Error: %s\n", strerror(errno));
            break;
        }
        while (!end_game) {
            char rec_buff[1];
            e = server_receive_message(&server, &peerskt, rec_buff, 1);
            if (e == -1) {
                printf("Error: %s\n", strerror(errno));
                break;
            }

            if (server_guess_hangman_letter(&server, rec_buff)) {
                end_game = true;
            }
  
            char len[3];
            char* disp_w = server_create_message_len(&server, end_game, len);
            e = server_send_message(&server, &peerskt, len, 3);
            if (e == -1) {
                printf("Error: %s\n", strerror(errno));
                break;
            }
            e = server_send_message(&server, &peerskt, disp_w, strlen(disp_w));
            if (e == -1) {
                printf("Error: %s\n", strerror(errno));
                break;
            }
            if (end_game) {
                socket_uninit(&peerskt);
            }
        }
    }
    server_print_recap(&server);
    server_uninit(&server);

    /*if (is_the_accept_socket_valid) {
        return 1;
    }
    else {*/
        return 0;
    //}
}
