#include "screen.h"
#include "low_level.h"


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
    offset /= 2; // Covert from cell offset to char offset.
    // This is similar to get_cursor, only now we write
    // bytes to those internal device registers.
    port_byte_out(REG_SCREEN_CTRL, 14);
    port_byte_out(REG_SCREEN_DATA, (unsigned char)(offset >> 8));
    port_byte_out(REG_SCREEN_CTRL, 15);
    port_byte_out(REG_SCREEN_DATA, offset);
}

void print_char( char character , int col , int row)
 {
    /* Create a byte ( char ) pointer to the start of video memory */
    unsigned char * vidmem = ( unsigned char *) VIDEO_ADDRESS ;
    char attribute_byte = WHITE_ON_BLACK;
    /* Get the video memory offset for the screen location */
    int offset = get_cursor();
    /* If col and row are non - negative , use them for offset . */
    // If we see a newline character , set offset to the end of
    // current row , so it will be advanced to the first col
    // of the next row.
    if ( character == '\n') {
        int rows = offset / (2* MAX_COLS );
        offset = get_screen_offset (79 , rows );
        // Otherwise , write the character and its attribute byte to
        // video memory at our calculated offset .
    } else {
        vidmem [ offset ] = character ;
        vidmem [ offset +1] = attribute_byte ;
    }
    // Update the offset to the next character cell , which is
    // two bytes ahead of the current cell .
    offset += 2;
    // Make scrolling adjustment , for when we reach the bottom
    // of the screen .
    //offset = handle_scrolling ( offset );
    // Update the cursor position on the screen device .
    set_cursor( offset );
}

static int prints(char **out, const char *string, int width, int pad)
{
	register int pc = 0, padchar = ' ';

	if (width > 0) {
		register int len = 0;
		register const char *ptr;
		for (ptr = string; *ptr; ++ptr) ++len;
		if (len >= width) width = 0;
		else width -= len;
		if (pad & PAD_ZERO) padchar = '0';
	}
	if (!(pad & PAD_RIGHT)) {
		for ( ; width > 0; --width) {
			printchar (out, padchar);
			++pc;
		}
	}
	for ( ; *string ; ++string) {
		printchar (out, *string);
		++pc;
	}
	for ( ; width > 0; --width) {
		printchar (out, padchar);
		++pc;
	}

	return pc;
}

static int printi(char **out, int i, int b, int sg, int width, int pad, int letbase)
{
	char print_buf[PRINT_BUF_LEN];
	register char *s;
	register int t, neg = 0, pc = 0;
	register unsigned int u = i;

	if (i == 0) {
		print_buf[0] = '0';
		print_buf[1] = '\0';
		return prints (out, print_buf, width, pad);
	}

	if (sg && b == 10 && i < 0) {
		neg = 1;
		u = -i;
	}

	s = print_buf + PRINT_BUF_LEN-1;
	*s = '\0';

	while (u) {
		t = u % b;
		if( t >= 10 )
			t += letbase - '0' - 10;
		*--s = t + '0';
		u /= b;
	}

	if (neg) {
		if( width && (pad & PAD_ZERO) ) {
			printchar (out, '-');
			++pc;
			--width;
		}
		else {
			*--s = '-';
		}
	}

	return pc + prints (out, s, width, pad);
}

static int print(char **out, int *varg)
{
	register int width, pad;
	register int pc = 0;
	register char *format = (char *)(*varg++);
	char scr[2];
	for (; *format != 0; ++format) {
		if (*format == '%') {
			++format;
			width = pad = 0;
			if (*format == '\0') break;
			if (*format == '%') goto out;
			if (*format == '-') {
				++format;
				pad = PAD_RIGHT;
			}
			while (*format == '0') {
				++format;
				pad |= PAD_ZERO;
			}
			for ( ; *format >= '0' && *format <= '9'; ++format) {
				width *= 10;
				width += *format - '0';
			}
			if( *format == 's' ) {
				register char *s = *((char **)varg++);
				pc += prints (out, s?s:"(null)", width, pad);
				continue;
			}
			if( *format == 'd' ) {
				pc += printi (out, *varg++, 10, 1, width, pad, 'a');
				continue;
			}
			if( *format == 'u' ) {
				pc += printi (out, *varg++, 10, 0, width, pad, 'a');
				continue;
			}
			if( *format == 'c' ) {
				/* char are converted to int then pushed on the stack */
				scr[0] = *varg++;
				scr[1] = '\0';
				pc += prints (out, scr, width, pad);
				continue;
			}
		}
		else {
		out:
			printchar (out, *format);
			++pc;
		}
	}
	if (out) **out = '\0';
	return pc;
}

void printchar(char **str, int c)
{
	if (str) {
		**str = c;
		++(*str);
	}
	else (void)print_char(c, -1, -1);
}

int printf(const char *format, ...)
{
	register int *varg = (int *)(&format);
	return print(0, varg);
}