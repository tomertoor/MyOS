#pragma once

#define KEYBOARD_DATA_PORT 0x60

unsigned int restart_keyboard();
unsigned char read_scan_code();
unsigned char getc();