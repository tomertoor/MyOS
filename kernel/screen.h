# define VIDEO_ADDRESS 0xb8000
# define MAX_ROWS 25
# define MAX_COLS 80

// Attribute byte for our default color
# define WHITE_ON_BLACK 0x0f
#define PAD_RIGHT 1
#define PAD_ZERO 2
#define PRINT_BUF_LEN 12
// Screen device I/O ports
# define REG_SCREEN_CTRL 0x3D4
# define REG_SCREEN_DATA 0x3D5

int printf(const char *format, ...);
void print_char ( char character , int col , int row);
void set_cursor(int offset);
int get_cursor();