/** @file ctx_switch_asm.S
 *
 * @brief Contains the core context switch routine.
 *
 * These routines all assume that the caller is following AAPCS, needs
 * no co-processor registers and that the caller is in SVC mode. Furthermore,
 * the SPSR is considered a spare/scratch register and hence, is not saved.
 *
 * @author Kartik Subramanian
 * @date   2008-11-21
 */

	.file "ctx_switch_asm.S"

#include <asm.h>
#include <arm/psr.h>

/**
 * @brief Special exit routine from the scheduler that launches a task for the
 * first time.
 *
 * r4 contains the user entry point.
 * r5 contains the single argument to the user function called.
 * r6 contains the user-mode stack pointer.
 * Upon completion, we should be in user mode.
 */
FUNC(launch_task)
	mov     r0, r5
	mov     r1, #0
	mov     r2, #0
	mov     r3, #0
	mov     r5, #0
	mov     r8, #0
	mov     ip, #0
	msr     cpsr_c, #(PSR_MODE_USR)
	mov     sp, r6
	mov     r6, #0
	ldr     lr, =0xdeadbeef   /* Causes a crash instead of calling the reset vector */
	mov     pc, r4

/* r0 points to the target context, r1 to the current context. */
/* add your code to perform a full context switch */
FUNC(ctx_switch_full)
	@ store current task registers to tcb
	stmia	r1, {r4-r11, sp, lr}
	@ load target register from tcb context
	b ctx_switch_half

/* r0 points to the target context */
/*add your code to perform a half context switch */	
FUNC(ctx_switch_half)
	@ load target registers from tcb context
	ldmia	r0, {r4-r11, sp, lr}
	@ -- For the first launch:
	@ the lr here is the lr set in context
	@ which is launch_task when task launched for the first time
	@ thus it branch to launch_task and execute task function
	@ -- After the first launch:
	@ lr should store the address to return to after context switch back 
	mov 	pc, lr
