@ crt0.S: Execution startup routine
@
@ Authors: Shi Su <shis@andrew.cmu.edu>
@          
@ Date:    Tue Oct 13 09:07:20 EDT 2015
#include <bits/swi.h>

	.file	"crt0.S"
	.text

	.global	_start
_start:
	ldr r0, [sp], #4 @ Load value argc from stack, and move the stack pointer 4 byte
    mov r1, sp       @ Send address of the sp(argv) to r1
    bl main          @ Call main function
    swi EXIT_SWI     @ Use the return value of main and exit
