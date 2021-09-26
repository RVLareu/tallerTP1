#include <stdio.h>

struct calculator_t;

typedef struct {

    char* (*calculator_hexToBin)(struct calculator_t *self, char* hex_number);
    int (*calculator_binToDec)(struct calculator_t *self, char* bin_number);

} calculator_t;

void calculator_init(calculator_t  *self);
void calculator_uninit(calculator_t  *self);
char* calculator_hexToBin(calculator_t *self, char* hex_number); 
int calculator_binToDec(calculator_t *self, char* bin_number);
