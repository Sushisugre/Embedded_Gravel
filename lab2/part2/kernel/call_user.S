@ call_user.S: 
@
@ Author: Shi Su <shis@andrew.cmu.edu>
@ Date:   

#include <bits/swi.h>

    .file   "call_user.S"
    .text

    .global call_user
call_user:
    stmfd sp!, {r0-r12, lr}  @store user program register and lr_svc

    @ Update set cpsr to usermode and masked FIQ/IRQ
    mrs r0, cpsr  @ copy cpsr to r0
    bic r0, r0, #0xff
    orr r0, r0, #0xd0 @ set F/T to 1, mode bit to user_mode
    msr cpsr, r0  @ copy the updated value back
    @ setup user stack
    @ sp is banked between svc and usr, so no need to save sp
    mov sp, #0xa3000000

    @ Jump to user program at 0xa2000000    
    mov r2, #0xa2000000
    blx r2  @ branch to the address in r

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

    @ somehow the exit value is not preserved

@ add a lable for exit syscall to jump to
@ return to kernel and clean up
    .global exit_user
exit_user:
    ldmfd sp!, {r0-r12, lr} @ restore registers
    movs pc, lr          @ return to kernel

    .section        .rodata
hello_str:
    .ascii  "Hello world!\n"
    .set    hello_size, .-hello_str @get the size of string
