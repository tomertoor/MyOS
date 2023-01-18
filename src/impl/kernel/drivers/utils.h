#pragma once

#include <stdint.h>

#define TERMINAL_ROOT "Ariel/Tomer@magshi-os$ "
#define EXIT_TERMINAL "exit"

int strncmp(const char* str1, const char* str2, int len);
void copy_buffer(char* source, char* dest, int no_bytes);
void sleep(int cycles);
void terminal();
void memset(void* dst, int c, int len);