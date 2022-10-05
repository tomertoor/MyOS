#pragma once

#define KEYBOARD_DATA_PORT 0x60
#define KEYBOARD_STATUS_PORT 0x64
#define RELEASED_SCAN_CODES_DIFFERENCE 0x80
#define MAX_STRING_LEN 50
#define MAX_INT_LEN 10

void keyboard();
unsigned int restart_keyboard();
unsigned char read_scan_code();
unsigned char getc();
unsigned char getchar();
void scan_string(char* str);
int scan_int();