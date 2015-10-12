@ read.S: read sycall wrapper
@
@ Authors: Shi Su <shis@andrew.cmu.edu>
@          
@ Date:    The current time & date

#include <bits/swi.h>
#include <bits/errno.h>
	.file	"read.S"
	.text

    @IMPORT  errno  @ import the global variable from C
	.global	read
read:
	swi  READ_SWI  @ read have 3 arguments, which are already in R0,R1,R2
    ldr  r1, =errno @ called function doesn't need to keep r0-r3
    str  r0, [r1]  @ store the return value into the address of errno
    bx  lr
    