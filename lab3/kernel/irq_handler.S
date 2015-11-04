@ irq_handler.S: IRQ handler in assembly
@                TBD
@
@ Author: Shi Su <shis@andrew.cmu.edu>
@         Mengjin Yan <mengjinyan@cmu.edu>
@
@ Date:   Wed Nov  4 11:13:25 EST 2015

    .file   "irq_handler.S"
    .text

    .global irq_handler

irq_handler:
    stmfd sp!, {r0-r12, lr}  @store user program register and lr_svc
    @ TBD!
    ldmfd sp!, {r0-r12, lr} 
    movs pc, lr         

