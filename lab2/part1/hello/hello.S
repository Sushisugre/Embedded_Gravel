@ hello.S: Hello world! in assembly
@
@ Authors: Group Member Shi Su <shis@andrew.cmu.edu>
@
@ Date:    The current time & October 8, 2015

#include <bits/swi.h>

	.file	"hello.S"
hello_str:
    .ascii  "Hello world!\n"
    .set hello_size, .-hello_str  @subtract the current address(.) from hello_str label
	.text
	.global	_start
_start:
    ldr r0, hello_str
    swi WRITE_SWI
	mov r0, #0
    swi EXIT_SWI

