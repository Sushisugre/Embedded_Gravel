@ Updated by: Shi Su, AndrewID: shis
@ Oct.01, 2015
@ Documentation of optimization locates in part2.txt
@
@ strTable function set each character in dst (characters that don't fit in the words got truncated)
@ to the position of its ascii number mod 23 in src


	.file	"part1-strTable.c"
	.text
	.align	2
	.global	strTable
	.type	strTable, %function
strTable:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	stmfd	sp!, {r4, r5}
	mov	r5, r0  @ r0 = src
@	mov	r4, r2  @ r4 = sl
	mov	r0, r3  @ r3 = dl
	subs r4, r0, #1  @ array index r4 = dl - 1
	blt	.L11  @ dl - 1 should >=0, dl >= 1
.L9:
	ldrb	r3, [r1, r4]	@ zero_extendqisi2 @ r3 = each character in dst 
	mov ip, r3
.L7:
	sub	ip, ip, #23
	cmp	ip, #22
	bgt	.L7
.L13:
	cmp	ip, r2 @ r2 = sl
	bgt	.L4  @ if position ip > sl, ignore
	strb	r3, [r5, ip]
.L4:
	subs r4, r4, #1  @ r4--
	bge	.L9  @ while r4 >= 0
.L11:
	ldmfd	sp!, {r4, r5} 
	mov pc, lr   @return
	.size	strTable, .-strTable
	.ident	"GCC: (GNU) 3.4.5"
