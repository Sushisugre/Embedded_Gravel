@ Updated by: Shi Su, AndrewID: shis
@ Sept.30, 2015
@
@ strTable function maps each character in dst 
@ to the position of its ascii number mod 23 in src


	.file	"part1-strTable.c"
	.text
	.align	2
	.global	strTable
	.type	strTable, %function
strTable:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	stmfd	sp!, {r4, r5, r6}
	mov	r5, r0  @ r0 = src
	mov	r4, r2  @ r4 = sl
	mov	r0, r3  @ r3 = dl
	mov	r2, #0  @ r2 is the index
	cmp r3, #0  @ dl must larger than 0
	ble	.L11
.L9:
	ldrb	r3, [r1, r2]	@ zero_extendqisi2 @ r3 = each character in dst E, 
	mov ip, r3
.L7:
	sub	ip, ip, #23
	cmp	ip, #22
	bgt	.L7
.L13:
	cmp	ip, r4
	bgt	.L4 @ if position ip > sl, ignore
	strb	r3, [r5, ip]
.L4:
	add	r2, r2, #1  @ r2++
	cmp	r2, r0   @ while r2 < dl
	blt	.L9
.L11:
	ldmfd	sp!, {r4, r5, r6} 
	mov pc, lr   @return
	.size	strTable, .-strTable
	.ident	"GCC: (GNU) 3.4.5"
