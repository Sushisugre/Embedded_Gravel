@ hello.S: Hello world! in assembly
@
@ Authors: Mengjin Yan <myan1@andrew.cmu.edu>
@ Date:    Mon 12 Oct 2015 05:54:46 PM 

#include <bits/swi.h>
#include <bits/fileno.h>
	
	.file	"hello.S"

	.text
	.global	_start
_start:
	@ Write "Hello world!\n" in stdout
	ldr	r0, =1
	adr	r1, hello_str
	mov	r2, #hello_size
	swi	WRITE_SWI

	@ Exit with status 0
	ldr	r0, =0
	swi	EXIT_SWI

hello_str:
	.ascii	"Hello world!\n"
	.set	hello_size, .-hello_str
