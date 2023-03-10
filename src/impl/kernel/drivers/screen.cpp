#include "screen.h"
#include "low_level.h"
#include "utils.h"

void set_cursor(int offset);
int get_screen_offset(int col, int row);
void print_at(char* message, int col, int row, char color);
int handle_scrolling(int cursor_offset);

int get_screen_offset(int col, int row) {
    return ((row * MAX_COLS) + col) * 2;
}

int get_cursor()
{
    port_byte_out ( REG_SCREEN_CTRL , 14);
    int offset = port_byte_in ( REG_SCREEN_DATA ) << 8;
    port_byte_out ( REG_SCREEN_CTRL , 15);
    offset += port_byte_in ( REG_SCREEN_DATA );
    return offset *2;
}

void set_cursor(int offset)
{
    offset /= 2;
    port_byte_out(REG_SCREEN_CTRL, 14);
    port_byte_out(REG_SCREEN_DATA, (unsigned char)(offset >> 8));
    port_byte_out(REG_SCREEN_CTRL, 15);
    port_byte_out(REG_SCREEN_DATA, offset);
}

void print_char( char character , int col , int row, char color)
 {
    unsigned char * vidmem = ( unsigned char *) VIDEO_ADDRESS ;
    char attribute_byte = WHITE_ON_BLACK;
    int offset = get_cursor();
    if ( character == '\n') {
        int rows = offset / (2* MAX_COLS );
        offset = get_screen_offset (79 , rows );
    }else if(character == '\t') {
        for(int i=0 ; i<4 ; i++)
        {
            vidmem [ offset ] = ' ' ;
            vidmem [ offset +1] = attribute_byte ;
            offset += 2;
        }
    } else {
        vidmem [ offset ] = character ;
        if(color)
            vidmem [ offset +1] = color ;
        else
            vidmem [ offset +1] = attribute_byte ;
    }
    offset += 2;
	offset = handle_scrolling(offset);
    set_cursor( offset );
}

void delete_char()
{
    unsigned char * vidmem = ( unsigned char *) VIDEO_ADDRESS;
    int offset = get_cursor();
    do
    {
        offset -= 2;
        vidmem [ offset] = '\0';
        vidmem [ offset+1] = WHITE_ON_BLACK;
    } while (vidmem[offset-2] == '\0');
	offset = handle_scrolling(offset);
    set_cursor( offset );
}

void print_at(char* message, int col, int row, char color) {
    if (col >= 0 && row >= 0) {
        set_cursor(get_screen_offset(col, row));
    }
    int i = 0;
    while (message[i] != 0) {
        print_char(message[i++], col, row, color);
    }
}

int handle_scrolling(int cursor_offset) {
    if (cursor_offset < MAX_ROWS*MAX_COLS*2) {
        return cursor_offset;
    }
    int i;
    for (i=1; i<MAX_ROWS; i++) {
        copy_buffer((char *)get_screen_offset(0, i) + VIDEO_ADDRESS,
                    (char *)get_screen_offset(0, i-1) + VIDEO_ADDRESS,
                    MAX_COLS*2
        );
    }
    char* last_line = (char *)get_screen_offset(0, MAX_ROWS-1) + VIDEO_ADDRESS;
    for (i=0; i<MAX_COLS*2; i++) {
        last_line[i] = 0;
    }
    cursor_offset -= 2*MAX_COLS;
    return cursor_offset;
}

void my_print(char* message, char color) {
    print_at(message, -1, -1, color);
}

void print_int(int num, char color)
{
    char arr[10]={0};
    int i = 0, r, neg=0;
    if(num<0)
    {
        neg=1;
        num*=-1;
    }
    while (num != 0) {
        r = num % 10;
        arr[i] = r+48;
        i++;
        num /= 10;
    }
    if(neg)
        print_char('-', -1, -1, color);
    for(int j=i-1 ; j>=0 ; j--)
    {
        print_char(arr[j], -1, -1, color);
    }
}

void cls() {
    int row = 0;
    int col = 0;
    for (row=0; row<MAX_ROWS; row++) {
        for (col=0; col<MAX_COLS; col++) {
            print_char('\0', col, row, 0);
        }
    }
    set_cursor(get_screen_offset(0, 0));
}