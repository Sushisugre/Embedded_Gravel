@ write.S: write sycall wrapper
@
@ Authors: Shi Su <shis@andrew.cmu.edu>
@          
@ Date:    Tue Oct 13 09:07:20 EDT 2015

#include <bits/swi.h>
#include <bits/errno.h>
	.file	"write.S"
	.text

	.global	write
write:  
    stmfd sp!, {r1-r12, lr}  @ save unbanked register before change mode
	swi WRITE_SWI  @ write have 3 arguments, which are already in R0,R1,R2
    ldr  r1, =errno @ called function doesn't need to keep r0-r3
    cmp  r0, #0     @ if return value < 0, error occurs, update errno
    bge  .return
    mov  r2, #0
    sub  r2, r2, r1 @ r2 = 0 - r1, get the absolute value of error code 
    str  r2, [r1]  @ store negate of the ERROR into the address of errno
    mov  r0, #-1   @ return value of C lib when error occurs
.return:    
    ldmfd sp!, {r1-r12, lr} 
    mov pc, lr
