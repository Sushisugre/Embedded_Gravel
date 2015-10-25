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

    @ Jump to user program at 0xa2000000
    
    @mov r2, #0xa2000000
    @bl r2

    @ test swi
    mov r0, #1
    ldr r1, =hello_str @load the address of label hello_str
    mov r2, #hello_size
    swi WRITE_SWI

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