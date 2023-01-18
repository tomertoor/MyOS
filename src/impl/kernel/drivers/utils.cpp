#include "utils.h"
#include "screen.h"
#include "keyboard.h"
#include "filesystem/filesystem.h"

void copy_buffer(char* source, char* dest, int no_bytes) {
    int i;
    for (i=0; i<no_bytes; i++) {
        *(dest + i) = *(source + i);
    }
}

void sleep(int cycles)
{
    for(int i = 0; i < cycles; i++)
    {
        __asm__("nop");
    }
}

int strncmp(const char* str1, const char* str2, int len)
{
    while ( len && *str1 && ( *str1 == *str2 ) )
    {
        ++str1;
        ++str2;
        --len;
    }
    if (len == 0)
        return 0;
    else
        return (*(unsigned char *)str1 - *(unsigned char *)str2);
}

void terminal()
{
    while(1)
    {
        my_print(TERMINAL_ROOT, 0x02);
        char choice[MAX_STRING_LEN] = {0};
        scan_string(choice);
        print_char('\n', -1, -1, 0);

        //here we should use the FILESYSTEM class to parse the differnet
        //user commands

        if(!strncmp(choice, EXIT_TERMINAL, MAX_STRING_LEN))
            break;
        else if(!strncmp(choice, "cls", MAX_STRING_LEN))
            cls();
        else
            my_print("Error, command not found\n", 0x04);
    }
    my_print("exit from terminal\n\n", 0);
}

void memset(void* dst, char c, size_t len)
{
    char* addr = (char*)dst;
    while (len != 0) {
        *addr++ = c;
        --len;
	}
}