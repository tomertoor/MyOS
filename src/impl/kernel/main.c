#include "filesystem/filesystemInterface.h"
#include "drivers/utils.h"
#include "drivers/screen.h"

#define FILESYSTEM

FSHandler createFSHandler(int num);
void freeFSHandler(FSHandler handler);
int printNum(FSHandler handler);
int printInode(FSHandler handler);

void main()
{
    cls();
    my_print("testing classes in C", 0);
    FSHandler handler = createFSHandler(12);
    my_print("working great!", 0);
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