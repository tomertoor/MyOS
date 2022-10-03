#include "keyboard.h"
#include "low_level.h"
#include "screen.h"

unsigned char chars[] = {
    '\0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
    '0', '-', '=', '\0', '\0', 'q', 'w', 'e', 'r', 't', //backspace then tab
    'y', 'u', 'i', 'o', 'p', '[', ']', '\0', '\0', 'a', //enter then left ctrl
    's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';'
};

unsigned int restart_keyboard()
{    
   unsigned char data = port_byte_in(0x61);     
   port_byte_out(0x61,data | 0x80);
   port_byte_out(0x61,data & 0x7F);
   return 0;
}

unsigned char read_scan_code()
{
    unsigned char code = port_byte_in(KEYBOARD_DATA_PORT);
    if((int)code <= 39)
    {
        return chars[code-1];
        port_byte_out(KEYBOARD_DATA_PORT, 0);
    }
    return 0;
}

unsigned char getc()
{
    int got = 0;
    while(!got)
    {
        got = read_scan_code();
    }
    return got;
}