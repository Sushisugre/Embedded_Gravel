/*
 * kernel.c: Kernel main (entry) function
 *
 * Author: Shi Su <shis@andrew.cmu.edu>
 *         Mengjin Yan <mengjinyan@cmu.edu>
 *
 * Date:   Wed Nov  4 11:07:45 EST 2015
 */

#include <exports.h>

#include <arm/psr.h>
#include <arm/exception.h>
#include <arm/interrupt.h>
#include <arm/timer.h>

#define WORD 4
#define LDR_BASE 0xe59ff000
#define LDR_PC_MINUS_4 0xe51ff004
#define LDR_MASK 0xfffff000
#define SWI_VECTOR 0x08
#define IRQ_VECTOR 0x18
#define E_BADCODE 0x0badc0de

uint32_t global_data;
/**
 * 
 */
uint32_t IRQ_STACK[30];

// swi handler in assembly
// get the swi num then transfer the control to c_swi_handler
extern void swi_handler(unsigned swi_num);
extern unsigned call_user(int argc, char *argv[]);

// irq handler
extern void irq_handler();

void install_handler(unsigned *old_handler, unsigned *new_handler);
void restore_handler(unsigned *old_handler, unsigned *old_inst);
unsigned* get_old_handler(unsigned* vector);

int kmain(int argc, char** argv, uint32_t table)
{
	app_startup(); /* bss is valid after this point */
	global_data = table; /* uboot function table */

	unsigned *old_swi_handler;
    unsigned old_swi_inst[2];
    unsigned *old_irq_handler;
    unsigned old_irq_inst[2];

    old_swi_handler = get_old_handler((unsigned*)SWI_VECTOR);
    if(old_swi_handler==0) return E_BADCODE;
    old_irq_handler = get_old_handler((unsigned*)IRQ_VECTOR);
    if(old_irq_handler==0) return E_BADCODE;
    
    // store the first 2 instructions of old swi handler
    old_swi_inst[0] = *(old_swi_handler);
    old_swi_inst[1] = *(old_swi_handler + 1);
    // store the first 2 instructions of old irq handler
    old_irq_inst[0] = *(old_irq_handler);
    old_irq_inst[1] = *(old_irq_handler+1);

    // install new handlers by modifying old ones
    install_handler(old_swi_handler, (unsigned*)&swi_handler);
    install_handler(old_irq_handler, (unsigned*)&irq_handler);

    // setup for usermode & call user program
    unsigned status = call_user(argc, argv);

    // restore native swi handler 
    restore_handler(old_swi_handler, old_swi_inst);
    restore_handler(old_irq_handler, old_irq_inst);

    return status;
}

/* calculate the address of old swi handler according to
 *  the ldr command in vector table */
unsigned* get_old_handler(unsigned* vector){
    unsigned offset, address;
    offset = (*vector) ^ LDR_BASE;

    // if swi vector doesn't contains ldr pc, [pc,#1mm12]
    if(((*vector)& LDR_MASK) != LDR_BASE){
        return 0;
    }
    
    // calculate the address of jumptable,
    // dereference it to get the address of swi handler
    address = *(unsigned*)((unsigned)vector + (2 * WORD) + offset);   

    return (unsigned*)address;
}

/* Change the first 2 instruction of native swi handler in uboot,
 to pass the control to the custom swi handler */
void install_handler(unsigned *old_handler, unsigned *new_handler){
    // load next instruction to pc when executing this line
    *old_handler = LDR_PC_MINUS_4;
    //  address of the new swi handler
    *(old_handler + 1) = (unsigned)new_handler;
}

/* restore the native swi handler to its original value */
void restore_handler(unsigned *old_handler, unsigned old_inst[]){
    // put the old swi handler instruction back to whare they were
    *old_handler = old_inst[0];
    *(old_handler + 1) = old_inst[1];
}
