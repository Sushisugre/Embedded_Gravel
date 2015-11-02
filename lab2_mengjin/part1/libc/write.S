@ write.S: write sycall wrapper
@
@ Authors: Mengjin Yan <myan1@andrew.cmu.edu>
@ Date:    Mon Oct 12 23:08:15 EDT 2015

#include <bits/swi.h>
	
	.file	"write.S"
	.text

	.global	write
write:
	stmfd	sp!, {r1-r12, lr}
	@ Write r2 size content from r1, to r0
	@ Since there are only 3 arguements, there is no need to convert
	@ conventions.
	swi	WRITE_SWI

	@ Check the return code
	ldmfd	sp!, {r1-r12, lr}
	cmp	r0, #0	
	bpl	writesuc

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

writesuc:
	@ Return
	mov	pc, lr
