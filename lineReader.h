#include <stdio.h>

/* 
    -stream: file with words to read
    -line: contains the word read
    -len: lenght of word

*/
typedef struct{
    FILE* stream;
    char *line;
    size_t len;
} lineReader_t;

/*creates lineReader*/
int lineReader_init(lineReader_t *self, FILE *stream);
/*destroys lineReader*/
int lineReader_uninit(lineReader_t *self);
/*reads a line*/
int lineReader_readLine(lineReader_t *self);
/*returns the last line read*/
char *lineReader_sendLine(lineReader_t *self);

