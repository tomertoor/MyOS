#include "drivers/screen.h"

void main()
{
    cls();
    char* msg ="testing\n\0";
    my_print(msg);
    
    char* msg2 = "Test\n\0";
    for (int i = 0; i<20; i++) {
        print_char('c', 0, 0);
    }
}