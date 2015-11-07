@ set_irq_stack.S: Put a global variable array in data section 
@                   as irq stack
@
@ Author: Shi Su <shis@andrew.cmu.edu>
@         Mengjin Yan <mengjinyan@cmu.edu>
@
@ Date: Fri Nov  6 22:17:34 EST 2015

#include <asm.h>
    .equ svc_mode_code, 0x53   @ enable irq in svc mode
    .equ irq_mode_code, 0xd2

FUNC(set_irq_stack)
    @ switch to irq mode
    @ Update set cpsr to irq mode and masked FIQ/IRQ
    mrs r2, cpsr  @ copy cpsr to r0
    bic r2, r2, #0xff
    orr r2, r2, #irq_mode_code
    msr cpsr, r2  @ copy the updated value back

    @ Set IRQ stack
    ldr r2, =g_irq_stack
    mov r3, #IRQ_STACK_HEIGHT
    @ sp = (stack address) + 4 * (stack size)
    add sp, r2, r3, LSL #2 

    @switch back to svc mode
    mrs r2, cpsr  
    bic r2, r2, #0xff
    orr r2, r2, #svc_mode_code @ set F/T to 1, mode bit to svc_mode
    msr cpsr, r2  @ copy the updated value back

    @ return
    mov pc, lr
