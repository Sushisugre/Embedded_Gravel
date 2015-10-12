@ hello.S: Hello world! in assembly
@
@ Authors: Shi Su <shis@andrew.cmu.edu>
@
@ Date:    Thu Oct  8 10:59:41 EDT 2015

#include <bits/swi.h>


	.file	"hello.S"
	.text
	.global	_start
_start:
    mov r0, #1  @output to std
    ldr r1, =hello_str @load the address of label hello_str
    mov r2, #hello_size
    swi WRITE_SWI
	mov r0, #0  @status code 
    swi EXIT_SWI


    .section        .rodata
hello_str:
    .ascii  "Hello world!\n"
    .set    hello_size, .-hello_str @get the size of string