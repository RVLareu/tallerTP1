#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lineReader.h"

/*
int main(int argc, char* argv[]) {
    lineReader_t lineReader;
    lineReader_init(&lineReader, fopen(argv[1],"r"));
    lineReader_readLine(&lineReader);
    char * line = lineReader_sendLine(&lineReader);
    printf("%s", line);
    lineReader_uninit(&lineReader);

}
*/

int lineReader_init(lineReader_t *self, FILE *stream) {
    self->stream = stream;
    self->line = NULL;
    self->len = 0;
    return 0;
}

int lineReader_uninit(lineReader_t *self) {
    free(self->line);
    fclose(self->stream);
    return 0; 
}

int lineReader_readLine(lineReader_t *self) {
    int s = getline(&self->line, &self->len, self->stream);
    self->line[strlen(self->line)-1] = '\0';
    return s; 
}

char *lineReader_sendLine(lineReader_t *self) {
    return self->line;
}



