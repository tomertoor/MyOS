#include "drivers/screen.h"
#include "drivers/keyboard.h"
#include "drivers/utils.h"

void main()
{
    cls();
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
    terminal();
    my_print("finished");
}