@ read.S: read sycall wrapper
@
@ Authors: Shi Su <shis@andrew.cmu.edu>
@          
@ Date:    The current time & date

#include <bits/swi.h>
#include <sys/errno.h>
	.file	"read.S"
	.text

    @IMPORT  errno  @ import the global variable from C
	.global	read
read:
	swi  READ_SWI  @ read have 3 arguments, which are already in R0,R1,R2
    ldr  r1, =errno @ called function doesn't need to keep r0-r3
    cmp  r1, #0     @ if return value < 0, error occurs, update errno
    bge  .return
    mov  r2, #0
    sub  r2, r2, r1 @ r2 = 0 - r1
    str  r2, [r1]  @ store negate of the ERROR into the address of errno
    mov  r0, #-1   @ return value of C lib when error occurs
.return:    
    bx  lr
