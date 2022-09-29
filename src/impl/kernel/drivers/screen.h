#define VIDEO_ADDRESS 0xb8000
#define MAX_ROWS 25
#define MAX_COLS 80
#define WHITE_ON_BLACK 0x0f
#define PAD_RIGHT 1
#define PAD_ZERO 2
#define PRINT_BUF_LEN 12
#define REG_SCREEN_CTRL 0x3D4
#define REG_SCREEN_DATA 0x3D5

void print_char ( char character , int col , int row);
void set_cursor(int offset);
int get_cursor();
int get_screen_offset(int col, int row);
void print_at(char* message, int col, int row);
void my_print(char* message);
void cls();
int handle_scrolling(int cursor_offset);