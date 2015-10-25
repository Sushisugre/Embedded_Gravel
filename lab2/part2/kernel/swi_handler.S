@ swi_handler.S: swi handler in assembly
@                get the swi num then transfer the control to c_swi_handler
@
@ Author: Shi Su <shis@andrew.cmu.edu>
@ Date:   Sat Oct 24 17:02:42 EDT 2015

    .file   "swi_handler.S"
    .text

    .global swi_handler

swi_handler:
    stmfd sp!, {r0-r12, lr}  @store user program register and lr_svc
    mov r1, sp          @ r1 is a pointer to the registers on stack
    ldr r0, [lr, #-4]   @ lr store the next instruction of SWI call, lr-4->SWI instruction
    bic r0, r0, #0xff000000  @clear first few bits to get swi number
    bl c_swi_handler    @ pass controll to C, r0 is swi number, r1 is pointer to register
    ldmfd sp!, {r0-r12} @ load the updated values on stack back to register
    movs pc, lr         @ return to swi wrapper

