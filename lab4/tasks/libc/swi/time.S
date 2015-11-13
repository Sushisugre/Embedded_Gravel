/** @file time.S
 *
 * @brief time sycall wrapper
 *
 * @author Shi Su <shis@andrew.cmu.edu>
 *         Mengjin Yan <mengjinyan@cmu.edu>
 *         
 * @date Thu Nov  5 23:08:44 EST 2015
 */

#include <asm.h>
#include <bits/swi.h>
#include <bits/errno.h>

        .file "time.S"

FUNC(time)
	    @ save unbanked register before change mode
        stmfd sp!, {r1-r12, lr}
        @ read have 3 arguments, which are already in R0,R1,R2  
        swi  TIME_SWI 
        @ called function doesn't need to keep r0-r3
        ldr  r1, =errno
        @ if return value < 0, error occurs, update errno
        cmp  r0, #0     
        bge  .return

        @ If the return value is negtive
        @ Extract the error number and set errno to the number
        @ And then set r0 to -1
        mov r4, #1
        ror r4, #1
        eor r0, r0, r4, asr #31
        add r0, r0, #1
        @ Get the address of errno
        ldr r4, =errno
        @ Store the new value of errno
        str r0, [r4]
        mov r0, #-1
.return:    
        ldmfd sp!, {r1-r12, lr} 
        mov pc, lr
