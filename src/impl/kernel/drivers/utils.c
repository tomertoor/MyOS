#include "utils.h"

void copy_buffer(char* source, char* dest, int no_bytes) {
    int i;
    for (i=0; i<no_bytes; i++) {
        *(dest + i) = *(source + i);
    }
}

void sleep(int cycles)
{
    for(int i = 0; i < cycles; i++)
    {
        __asm__("nop");
    }
}