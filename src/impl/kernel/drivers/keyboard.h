#pragma once

#define KEYBOARD_DATA_PORT 0x60
#define KEYBOARD_STATUS_PORT 0x64
#define RELEASED_SCAN_CODES_DIFFERENCE 0x80


unsigned int restart_keyboard();
unsigned char read_scan_code();
unsigned char getc();