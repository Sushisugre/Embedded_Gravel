@ call_user.S: Setup cpsr, stack for usermode
@ and call the user program
@
@ Author: Shi Su <shis@andrew.cmu.edu>
@         Mengjin Yan <mengjinyan@cmu.edu>
@
@ Date:   Mon Oct 26 14:17:08 EDT 2015

#include <bits/swi.h>
#include <asm.h>


    .file   "call_user.S"
    .text
    .equ user_entry, 0xa0000000  
    .equ user_stack, 0xa3000000
    .equ svc_stack_backup, 0xa2000000  @ might be faultful
    .equ user_mode_code, 0x50  @ eable irq in user mode
    .equ svc_mode_code, 0x53   @ enable irq in svc mode
    .equ null, 0x0

FUNC(call_user)
    stmfd sp!, {r1-r12, lr}  @store user program register and lr_svc

    @ setup user stack
    @ strore the svc stack pointer
    adr   r2, =g_svc_stack
    str   sp, [r2] 

    @ Update set cpsr to usermode and masked FIQ/IRQ
    mrs r2, cpsr  @ copy cpsr to r0
    bic r2, r2, #0xff
    orr r2, r2, #user_mode_code @ set F to 1, I to 0 (enable irq), mode bit to user_mode
    msr cpsr, r2  @ copy the updated value back

    mov sp, #user_stack
    mov r2, #null  @ null
    stmdb sp!, {r2}
    mov r2, r0 @ argc, number of arguments
.set_argv:
    subs r2, r2, #1  @ loop argc times to put those argument on stack
    BMI .call_function @ <0
    ldr   r3, [r1, r2, LSL #2]  @ r1(argv)+4(i-1),+4(i-2), ...4
    stmdb sp!, {r3} @store the variables
    B .set_argv

    @ Jump to user program at 0xa2000000  
.call_function:
    stmdb sp!, {r0}  @ set argc  
    mov r2, #user_entry
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
    @mov   r2, #svc_stack_backup
    ldr   r2, =g_svc_stack
    ldr   sp, [r2]
    ldmfd sp!, {r1-r12, lr} @ restore registers
    mov pc, lr          @ return to kernel, no s! not switching mode!

@@@@@@@@@@@@@@@@@@@@@ TEST CODE PLEASE IGNORE @@@@@@@@@@@@@@@@@@@@@@@@@@@@@

    @ test write swi
    @ mov r0, #1
    @ ldr r1, =hello_str @load the address of label hello_str
    @ mov r2, #hello_size
    @ swi WRITE_SWI

    @ test read swi
    @ mov r0, #0
    @ mov r1, #0xa2000000
    @ mov r2, #10
    @ swi READ_SWI

    @ test exit swi
    @ mov r0, #42
    @ swi EXIT_SWI

   @ .section        .rodata
@ hello_str:
   @ .ascii  "Hello world!\n"
   @ .set    hello_size, .-hello_str @get the size of string
