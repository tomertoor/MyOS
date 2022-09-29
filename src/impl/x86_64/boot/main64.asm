global main_start
extern main

section .text
bits 32
main_start:
    call main
    hlt