#include "drivers/utils.h"
#include "drivers/screen.h"
#include "filesystem/filesystem.h"

void main()
{
    cls();
    Filesystem a(45);
    print_int(a.getInode(), 0x0);
    my_print("\n", 0x0);
    print_int(a.getNum(), 0x0);
    my_print("Working beautifully", 0x0);
    cls();
    terminal();
}


// keyboard();
    // my_print("Enter your char: ");
    // char a = getchar();
    // my_print("getchar returned --> ");
    // print_char(a, -1, -1);
    // my_print("\n\n");

    
    // char str[MAX_STRING_LEN] = {0};
    // my_print("Enter your string: ");
    // scan_string(str);
    // my_print("\n");
    // my_print("Your string is: ");
    // my_print(str);
    // my_print("\n\n");

    // my_print("Enter your integer: ");
    // int num = scan_int();
    // my_print("\n");
    // my_print("Your integer is: ");
    // print_int(num);
    // my_print("\n\n");