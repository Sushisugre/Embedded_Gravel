@ hello.S: Hello world! in assembly
@
@ Authors: Group Member Shi Su <shis@andrew.cmu.edu>
@
@ Date:    Thu Oct  8 10:59:41 EDT 2015

#include <bits/swi.h>

	.file	"hello.S"

	.text
	.global	_start
_start:
    ldr r0, =hello_str @load the address of label hello_str
    mov r1, #hello_size
    swi WRITE_SWI
	mov r0, #0
    swi EXIT_SWI

    .section    .rodata
hello_str:
    .ascii  "Hello world!\n"
    .set hello_size, .-hello_str  @subtract the current address(.) from hello_str label

