/** @file read.S
 *
 * @brief read sycall wrapper
 *
 * @author Mike Kasick <mkasick@andrew.cmu.edu>
 * @date   Sun, 07 Oct 2007 01:53:16 -0400
 * 
 * @author Kartik Subramanian <ksubrama@andrew.cmu.edu>
 * @date 2008-10-29
 */

#include <asm.h>
#include <bits/swi.h>
#include <bits/errno.h>

	.file "read.S"

    .global read
FUNC(read)
    stmfd sp!, {r1-r12, lr}  @ save unbanked register before change mode
    swi  READ_SWI  @ read have 3 arguments, which are already in R0,R1,R2
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

