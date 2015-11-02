@ s_handler.S: Assembly part in swi handler
@
@ Author: Mengjin Yan <myan1@andrew.cmu.edu>
@ Date:   Mon Oct 26 16:19:59 EDT 2015

	.file	"s_handler.S"
	.text

	.global	S_Handler

S_Handler:
	stmfd	sp!, {r0-r12, lr}	@ Store user's gp registers, lr_svc
	ldr	r1, =0xa3000008		@ restore r8
	ldr	r8, [r1]
	mov	r1, sp			@ r1 now contains pointer to parameters on stack
	ldr	r0, [lr, #-4]		@ extract the SWI number
	bic	r0, r0, #0xff000000	@ get SWI # in r0 by bit-masking
	bl	C_SWI_handler		@ go to handler
	ldmfd	sp!, {r0-r12, lr}	@ Restore user's gp register, lr_svc
	movs	pc, lr			@ return from handler

