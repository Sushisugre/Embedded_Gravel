@ hello.S: Hello world! in assembly
@
@ Authors: Group Member Shi Su <shis@andrew.cmu.edu>
@
@ Date:    The current time & October 8, 2015

#include <bits/swi.h>

	.file	"hello.S"
hello_str:
    .string "Hello world\n"
	.text
	.global	_start
_start:
    ldr r0, hello_str
    swi WRITE_SWI
	mov r0, #0
    swi EXIT_SWI

