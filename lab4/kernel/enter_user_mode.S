@ enter_user_mode.S: Setup cpsr, stack for usermode
@ and call the user program
@
@ Author: Shi Su <shis@andrew.cmu.edu>
@         Mengjin Yan <mengjinyan@cmu.edu>
@
@ Date:   Mon Oct 26 14:17:08 EDT 2015

#include <bits/swi.h>
#include <asm.h>
#include <config.h>


    .file   "enter_user_mode.S"
    .text
    .equ user_mode_code, 0x10  @ eable irq in user mode
    .equ svc_mode_code, 0x13   @ enable irq in svc mode
    .equ null, 0x0

FUNC(enter_user_mode)
    stmfd sp!, {r1-r12, lr}  @store user program register and lr_svc

    @ setup user stack
    @ strore the svc stack pointer
    ldr   r2, =g_svc_stack
    str   sp, [r2] 

    @ Update set cpsr to usermode and masked FIQ/IRQ
    mrs r2, cpsr  @ copy cpsr to r0
    bic r2, r2, #0xff
    orr r2, r2, #user_mode_code @ set F to 1, I to 0 (enable irq), mode bit to user_mode
    msr cpsr, r2  @ copy the updated value back

    @ Setup user mode stack
    mov sp, #USR_STACK
    @ Jump to user program 
.call_function:
    mov r2, #LOAD_ADDR
    blx r2  @ branch to the address in r

    @ restore svc mode
    mrs r2, cpsr  @ copy cpsr to r0
    bic r2, r2, #0xff
    orr r2, r2, #svc_mode_code @ set F/T to 1, mode bit to svc_mode
    msr cpsr, r2  @ copy the updated value back

@ add a lable for exit syscall to jump to
@ return to kernel and clean up
    .global exit_user
exit_user:
    ldr   r2, =g_svc_stack
    ldr   sp, [r2]
    ldmfd sp!, {r1-r12, lr} @ restore registers
    mov pc, lr          @ return to kernel, no s! not switching mode!


