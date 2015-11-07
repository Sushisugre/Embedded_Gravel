@ irq_handler.S: IRQ handler in assembly
@
@ Author: Shi Su <shis@andrew.cmu.edu>
@         Mengjin Yan <mengjinyan@cmu.edu>
@
@ Date:   Wed Nov  4 11:13:25 EST 2015

#include <asm.h>
#include <config.h>

FUNC(irq_handler)

	@ Store user program register and lr_irq
	stmfd	sp!, {r0-r12, lr}

	@ Call c_irq_handler
	bl	c_irq_handler

	@ Load the value back to registers
	ldmfd	sp!, {r0-r12, lr} 
	subs	pc, lr, #4         

