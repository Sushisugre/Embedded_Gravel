@ user_function.S: Switch to user mode
@
@ Author: Mengjin Yan <myan1@andrew.cmu.edu>
@ Date:	  Mon Oct 26 17:09:46 EDT 2015

	.file	"user_function.S"
	.text

	.global	User_Function
	.global exit

User_Function:
	@ Push non banked register into svc stack
	stmfd	sp!, {r1-r12, lr}
	ldr	r2, =0xa3000004
	str	sp, [r2]
	ldr	r2, =0xa3000008
	str	r8, [r2]
	
	@ Switch to user mode with IRQs & FIOs masked
	mrs	r2, cpsr
	ldr	r3, =0xFFFFFFD0
	and	r2, r2, r3
	msr	cpsr, r2
	
	@ Setup a full descending user mode stack
	@ (with the stack top at 0xa3000000)
	ldr	sp, =0xa3000000
	
	@ Push U-boot's argc & argv on the user stack
	stmfd	sp!, {r0, r1}
	
	@ Jump to a loaded user program at address 0xa2000000
	ldr	r0, =0xa2000000
	blx	r0

exit:	
	@ Restore the non banked register
	ldr	r1, =0xa3000004
	ldr	sp, [r1]
	ldmfd 	sp!, {r1-r12, lr}
	
	@ return to the main kernel function
	movs	pc, lr