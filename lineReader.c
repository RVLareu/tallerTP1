#include <stdio.h>
#include <stdlib.h>
#include "lineReader.h"

int lineReader_init(lineReader_t *self, FILE *stream) {
    self->stream = stream;
    self->line = NULL;
    self->len = 0;
    return 0;
}

int lineReader_uninit(lineReader_t *self) {
    free(self->line);
    return 0; 
}

int lineReader_readLine(lineReader_t *self) {
    return getline(&self->line, &self->len, self->stream);
    
}

char *lineReader_sendLine(lineReader_t *self) {
    return self->line;
}



