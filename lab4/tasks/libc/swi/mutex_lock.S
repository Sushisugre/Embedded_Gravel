/** @file mutex_lock.S
 *
 * @brief mutex lock sycall wrapper
 *
 * @author Shi Su <shis@andrew.cmu.edu>
 *         Mengjin Yan <mengjinyan@cmu.edu>
 *         
 * @date 2015-11-24
 */

#include <asm.h>
#include <bits/swi.h>
#include <bits/errno.h>

	.file "mutex_lock.S"

FUNC(mutex_lock)
        stmfd sp!, {r1-r12, lr}  @ save unbanked register before change mode
        swi  MUTEX_LOCK 
        ldr  r1, =errno @ called function doesn't need to keep r0-r3
        cmp  r0, #0     @ if return value < 0, error occurs, update errno
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
