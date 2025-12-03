#include <stdio.h>
#include <assert.h>

void log(char *message) {
    #ifdef DEBUG
        fprintf(stderr, message);
    #else
        return;
    #endif
}