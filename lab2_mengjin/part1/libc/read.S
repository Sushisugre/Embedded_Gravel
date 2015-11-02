@ read.S: read sycall wrapper
@
@ Authors: Mengjin Yan <myan1@andrew.cmu.edu>
@ Date:    Mon 12 Oct 2015 08:30:29 PM EDT

#include <bits/swi.h>
	
	.file	"read.S"
	.text

	.global	read
read:
	stmfd	sp!, {r1-r12, lr}
	@ Read r2 size content from r0, to r1
	@ Since there are only 3 arguements, there is no need to convert
	@ converntions.
	swi	READ_SWI

	@ Check the return code
	ldmfd	sp!, {r1-r12, lr}
	cmp	r0, #0
	bpl	readsuc

	@ If the return value is negtive
	@ Extract the error number and set errno to the number
	@ And then set r0 to -1
	mov	r4, #1
	ror	r4, #1
	eor	r0, r0, r4, asr #31
	add	r0, r0, #1
	@ Get the address of errno
	ldr	r4, =errno
	@ Store the new value of errno
	str	r0, [r4]
	mov	r0, #-1
	
readsuc:
	@ Return
	mov	pc, lr
	
