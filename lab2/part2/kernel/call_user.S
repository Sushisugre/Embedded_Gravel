@ call_user.S: 
@
@ Author: Shi Su <shis@andrew.cmu.edu>
@ Date:   

#include <bits/swi.h>

    .file   "call_user.S"
    .text

    .global call_user
call_user:
    @stmfd sp!, {r0-r12, lr}  @store user program register and lr_svc
    
    @ Update set cpsr to usermode and masked FIQ/IRQ

    @ Jump to user program at 0xa2000000
    
    @mov r2, #0xa2000000
    @bl r2

    swi WRITE_SWI

@ add a lable for exit syscall to jump to
@ return to kernel and clean up
.exit_user:
    @ldmfd sp!, {r0-r12} @ restore registers
    movs pc, lr          @ return to kernel