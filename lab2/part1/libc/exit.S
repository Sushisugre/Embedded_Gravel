@ exit.S: exit sycall wrapper
@
@ Authors: Shi Su <shis@andrew.cmu.edu>
@          
@ Date:    Thu Oct  8 19:49:59 EDT 2015

#include <bits/swi.h>

	.file	"exit.S"
	.text

	.global	exit
exit:                @ APCS and OABI use same register r0 for argument 1
	swi    EXIT_SWI  @ r0 is already set by calling program
                     @ exit has no return value
    swi WRITE_SWI  @ write have 3 arguments, which are already in R0,R1,R2
    LDR  R1, =errno @ called function doesn't need to keep r0-r3
    STR  R0, [R1]  @ store the return value into the address of errno