#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lineReader.h"


/*creates a lineReader, setting a stream*/
int lineReader_init(lineReader_t *self, FILE *stream) {
    self->stream = stream;
    self->line = NULL;
    self->len = 0;
    return 0;
}
/*destroys lineReader, freeing the line and closing the file*/
int lineReader_uninit(lineReader_t *self) {
    free(self->line);
    fclose(self->stream);
    return 0; 
}
/*reads line with getline()*/
int lineReader_readLine(lineReader_t *self) {
    int s = getline(&self->line, &self->len, self->stream);
    self->line[strlen(self->line)-1] = '\0';
    return s; 
}
/*returns last line read*/
char *lineReader_sendLine(lineReader_t *self) {
    return self->line;
}



