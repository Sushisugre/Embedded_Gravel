@ crt0.S: Execution startup routine
@
@ Authors: Shi Su <shis@andrew.cmu.edu>
@          
@ Date:    The current time & date
#include <bits/swi.h>

	.file	"crt0.S"
	.text

	.global	_start
_start:
	ldr r0, [sp], #4 @ sp -> argc
    mov r1, sp  @ sp+4 -> address argv
    bl main         @ Call main function
    swi EXIT_SWI    @ Use the return value of main and exit
