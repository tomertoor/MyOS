#include "keyboard.h"
#include "low_level.h"
#include "screen.h"

unsigned char chars[] = {
    '\0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
    '0', '-', '=', '\b', '\t', 'q', 'w', 'e', 'r', 't', 
    'y', 'u', 'i', 'o', 'p', '[', ']', '\n', '\0', 'a', //left ctrl
    's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'',
    '\0', '\0', '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', //backtick then left shift
    ',', '.', '/', '\0', '\0', '\0', ' ' //right shift then keypad* then left alt
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
    char state = port_byte_in(KEYBOARD_STATUS_PORT);
    
    if(state & 1) // checks if the output buffer status if its full or not.
    {
        unsigned char code = port_byte_in(KEYBOARD_DATA_PORT);
        if((int)code <= 57)// && last_key != code)
        {
            port_byte_out(KEYBOARD_DATA_PORT, 0);
            return chars[code-1];
        }
    }
    return 0;
}

/*
testing the keyboard functionality
*/
void keyboard()
{
    while(1)
    {
        char test = getc();
        if(test == '\b')
            delete_char();
        else
            print_char(test, -1, -1, 0);
        __asm__("nop");
    }
}

/*
the getchar function remake
*/
unsigned char getchar()
{
    char a=' ', b=' ';
    int originalOffest = get_cursor();
    while(1)
    {
        a = getc();
        if(a == '\n')
            break;
        else if(a == '\b')
        {
            if(originalOffest < get_cursor())
                delete_char();
        }
        else
        {
            b = a;
            print_char(b, -1, -1, 0);
        }
    }
    my_print("\n", 0);
    return b;
}

/*
the getc function remake
*/
unsigned char getc()
{
    int got = 0;
    while(!got)
    {
        got = read_scan_code();
    }
    return got;
}

/*
writes the characters typed to the string buffer
maximus size in MAX_STRING_SIZE
*/
void scan_string(char* str)
{
    int i = 0, originalOffest = get_cursor();
    while(1)
    {
        char a = getc();
        if(a == '\n')
            break;
        else if(a == '\b')
        {
            if(originalOffest < get_cursor())
            {
                delete_char();
                str[--i] = '\0';
            }
        }
        else
        {
            print_char(a, -1, -1, 0);
            if(i<MAX_STRING_LEN-1) //the last '\0' must be
            {
                str[i] = a;
                i++;
            }
        }
    }
}

/*
returns -1 in case of invalid integer
otherwise returns the integer entered
*/
int scan_int()
{
    char num[MAX_INT_LEN] = {0};
    int i = 0, originalOffest = get_cursor();
    while(1)
    {
        char a = getc();
        if(a == '\n')
            break;
        else if(a == '\b')
        {
            if(originalOffest < get_cursor())
            {
                delete_char();
                num[--i] = '\0';
            }
        }
        else
        {
            if(i<MAX_INT_LEN)
            {
                num[i] = a;
                print_char(num[i], -1, -1, 0);
                i++;
            }
        }
    }
    int number = 0;
    int minus = 1;
    i=0;
    if (num[0] == '-') 
    {
        minus = -1;
        i++;
    }
    for (; num[i] != '\0'; ++i)
    {
        if(num[i] >= '0' && num[i] <= '9')
            number = number * 10 + num[i] - '0';
        else
            return -1;
    }
    return number * minus;
}