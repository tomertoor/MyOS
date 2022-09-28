global start

section .text
bits 32

char:
	db 0


start:
	;print:
	;mov ah, 0
	;int 0x16

	mov al, [char]
	mov dword [0xb8000], 0x2f4f2f
	;mov ah, 0x0e
	;mov al, 'C'
	;int 0x10
	hlt
