#include <stdio.h>




typedef struct{
    FILE* stream;
    char *line;
    size_t len;
} lineReader_t;


int lineReader_init(lineReader_t *self, FILE *stream);
int lineReader_uninit(lineReader_t *self);

int lineReader_readLine(lineReader_t *self);

char *lineReader_sendLine(lineReader_t *self);

