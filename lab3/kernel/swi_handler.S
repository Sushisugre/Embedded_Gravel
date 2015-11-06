@ swi_handler.S: swi handler in assembly
@                get the swi num then transfer the control to c_swi_handler
@
@ Author: Shi Su <shis@andrew.cmu.edu>
@         Mengjin Yan <mengjinyan@cmu.edu>
@
@ Date:   Fri Nov  6 00:47:45 EST 2015

        .file   "swi_handler.S"
        .text

        .global swi_handler

swi_handler:
        @ store user program register and lr_svc
        stmfd sp!, {r0-r12, lr} 
        @ restore svc r8 from global variable
        ldr r8, =global_data
        @ r1 is a pointer to the registers on stack
        mov r1, sp          
        @ lr store the next instruction of SWI call, lr-4->SWI instruction
        ldr r0, [lr, #-4]
        @clear first few bits to get swi number   
        bic r0, r0, #0xff000000
        @ pass controll to C, r0 is swi number, r1 is pointer to register  
        bl c_swi_handler  
        @ load the updated values on stack back to register  
        ldmfd sp!, {r0-r12, lr}
        @ return to swi wrapper 
        movs pc, lr         

