

#include <string.h>
#include <stdio.h>
#include <errno.h>

#include <stdbool.h>


#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>

#include <stdlib.h>
#include "clientTda.h"

/*main app from clients side*/
int main(int argc, char* argv[]) {
    const char *hostname = argv[1];
    const char *servicename = argv[2];
    bool is_there_a_socket_error = false;

    client_t client;
    client_init_connect(&client, hostname, servicename);
    /*firs message from server*/
    char buf_first_message[3] = "";
    int size_first_m = 3;
    client_receive_process(&client, buf_first_message, size_first_m, 0);

    bool end_game = false;
    /*INGAME LOOP*/
    while (!end_game && !is_there_a_socket_error) {
        /*input from player*/
        char buffer[30] = "";
        int e = scanf("%29s", buffer);  
        if (e != 1) {
            client_uninit(&client);
            return 1;
        }
        for (int i = 0; i < strlen(buffer) && !end_game; i++) {
            /*---SEND---*/
            e = client_send(&client, &buffer[i], 1);
            if (e == -1) {
                printf("Error: %s\n", strerror(errno));
                is_there_a_socket_error = true;
                break;
            }

            /*---RECEIVE---*/
            char buf[3] = "";
            int size = 3;
            if (client_receive_process(&client, buf, size, 1)) {
                end_game = true;
                client_uninit(&client);
                break;
            }
        }
    }
    client_uninit(&client);
    
    if (is_there_a_socket_error) {
        return 1;  
    } else {
        return 0;
    }
}    
