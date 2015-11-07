@ irq_handler.S: IRQ handler in assembly
@                TBD
@
@ Author: Shi Su <shis@andrew.cmu.edu>
@         Mengjin Yan <mengjinyan@cmu.edu>
@
@ Date:   Wed Nov  4 11:13:25 EST 2015

#include <asm.h>
#include <config.h>

FUNC(irq_handler)
	@ Set IRQ stack
	ldr	r2, =g_irq_stack
	mov r3, #IRQ_STACK_HEIGHT
	@ sp = (stack address) + 4 * (stack size)
	add sp, r2, r3, LSL #2 

	@ Store user program register and lr_svc
	stmfd	sp!, {r0-r12, lr}

	@ Call c_irq_handler
	bl	c_irq_handler

	@ Load the value back to registers
	ldmfd	sp!, {r0-r12, lr} 
	subs	pc, lr, #4         

