#include "drivers/screen.h"
#include "drivers/keyboard.h"

void main()
{
    cls();
    // my_print("Hello world!\nMy name is Ariel and this is an os!\n");
    // char* test = "testing\0";
    // for(int i = 0 ; i <20 ; i++)
    // {
    //     my_print(test);
    // }
    while(1)
    {
        char test = getc();

        print_char(test, -1, -1);
        __asm__("nop");
        //my_print("DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD");
    }
    
    my_print("\n\n");
    my_print("finished");
}