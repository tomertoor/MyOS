#include "drivers/screen.h"

void main()
{
    cls();
    my_print("Hello world!\nMy name is Ariel and this is an os!\n");
    char* test = "testing\0";
    for(int i = 0 ; i <20 ; i++)
    {
        my_print(test);
    }
}