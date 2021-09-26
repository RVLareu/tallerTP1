#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "calculator.h"


void calculator_init(calculator_t *self) {
    self->calculator_hexToBin = calculator_hexToBin;
    self->calculator_binToDec = calculator_hexToBin;
}

void calculator_uninit(calculator_t *self) {

}

char* calculator_hexToBin(calculator_t *self, char* hex_number) {
    char* binNumber = malloc(sizeof(hex_number)*strlen(hex_number)/2);
    for (int i = 0; i < strlen(hex_number); i++) {
       if (hex_number[i] == '0') {
            strcat(binNumber,"0000");
            continue;
        } 
        if (hex_number[i] == '1') {
            strcat(binNumber,"0001");
            continue;
        }
        if (hex_number[i] == '2') {
            strcat(binNumber,"0010");
            continue;
        }
        if (hex_number[i] == '3') {
            strcat(binNumber,"0011");
            continue;
        }
        if (hex_number[i] == '4') {
            strcat(binNumber,"0100");
            continue;
        }
        if (hex_number[i] == '5') {
            strcat(binNumber,"0101");
            continue;
        }
        if (hex_number[i] == '6') {
            strcat(binNumber,"0110");
            continue;
        }
        if (hex_number[i] == '7') {
            strcat(binNumber,"0111");
            continue;
        }
        if (hex_number[i] == '8') {
            strcat(binNumber,"1000");
            continue;
        }
        if (hex_number[i] == '9') {
            strcat(binNumber,"1001");
            continue;
        }
        if (hex_number[i] == 'a') {
            strcat(binNumber,"1010");
            continue;
        }
        if (hex_number[i] == 'b') {
            strcat(binNumber,"1011");
            continue;
        }
        if (hex_number[i] == 'c') {
            strcat(binNumber,"1100");
            continue;
        }
        if (hex_number[i] == 'd') {
            strcat(binNumber,"1101");
            continue;
        }
        if (hex_number[i] == 'e') {
            strcat(binNumber,"1110");
            continue;
        }
        if (hex_number[i] == 'f') {
            strcat(binNumber,"1111");
            continue;
        }

    }
    return binNumber;
}

int calculator_binToDec(calculator_t *self, char* bin_number) {
    int binary_number = atoi(bin_number);
    int dec_number = 0;
    int base = 1; 
      
    while (binary_number > 0) {
        dec_number = dec_number + (binary_number % 10) * base;  
        binary_number = binary_number / 10;
        base = base * 2;  
    } 
    return dec_number;
}  

