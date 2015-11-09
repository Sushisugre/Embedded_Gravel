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
#include <arm/reg.h>
#include <config.h>

#define WORD 4
#define LDR_BASE 0xe59ff000
#define LDR_PC_MINUS_4 0xe51ff004
#define LDR_MASK 0xfffff000
#define SWI_VECTOR 0x08
#define IRQ_VECTOR 0x18
#define E_BADCODE 0x0badc0de

uint32_t global_data;

/**
 *  IRQ stack which will be placed in the data section
 */
uint32_t g_irq_stack[IRQ_STACK_HEIGHT];

/**
 * Saved svc stack pointer to data section
 */
uint32_t g_svc_stack = 0;

/**
 *  Time Counters
 */
// millisecond
volatile unsigned g_ms_counter; 
// second
volatile unsigned g_s_counter;
// minute
volatile unsigned g_m_counter;
// hour
volatile unsigned g_h_counter;

/**
 * swi handler in assembly
 * get the swi num then transfer the control to c_swi_handler
 */
extern void swi_handler(unsigned swi_num);

/**
 * Setup environment and branch to usermode 
 */
extern unsigned call_user(int argc, char *argv[]);

/**
 * IRQ handler
 */
extern void irq_handler();

/**
 * Set IRQ stack
 */
extern void set_irq_stack();

void install_handler(unsigned *old_handler, unsigned *new_handler);
void restore_handler(unsigned *old_handler, unsigned *old_inst);
unsigned* get_old_handler(unsigned* vector);
void update_interrupt_controller(unsigned icmr, unsigned iclr);
void init_timer();

int kmain(int argc, char** argv, uint32_t table)
{
	app_startup(); /* bss is valid after this point */
	global_data = table; /* uboot function table */

	unsigned *old_swi_handler;
    unsigned *old_irq_handler;
    unsigned old_swi_inst[2];
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

    set_irq_stack();

    unsigned old_icmr = reg_read(INT_ICMR_ADDR);
    unsigned old_iclr = reg_read(INT_ICLR_ADDR);
    update_interrupt_controller(1 << INT_OSTMR_0, 0);
    init_timer();

    // setup for usermode & call user program
    unsigned status = call_user(argc, argv);

    // restore native swi/irq handler, interrupt controller
    restore_handler(old_swi_handler, old_swi_inst);
    restore_handler(old_irq_handler, old_irq_inst);
    update_interrupt_controller(old_icmr, old_iclr);

    return status;
}

void update_interrupt_controller(unsigned icmr, unsigned iclr) {
    // mask all devices except OSMR0 in ICMR
    // reg_write(INT_ICMR_ADDR, 0x04000000);
    reg_write(INT_ICMR_ADDR, icmr);

    // set OSMR0 to generate IRQ in ICLR
    // other devices are masked so the value in ICLR has no effect on them
    reg_write(INT_ICLR_ADDR, iclr);
}


void init_timer() {

    // clear global counters
    g_ms_counter = 0;
    g_s_counter = 0;
    g_m_counter = 0;
    g_h_counter = 0;

    // Clear OSSR
    reg_set(OSTMR_OSSR_ADDR, OSTMR_OSSR_M0);
    // enable OSMR0 in OIER
    reg_write(OSTMR_OIER_ADDR, OSTMR_OIER_E0);
    // set the value of OS Timer Counter Register to 0
    reg_write(OSTMR_OSCR_ADDR, 0);
    // set the value of OS Timer Counter Register to 10 milisecond
    reg_write(OSTMR_OSMR_ADDR(0), OS_TIMER_INTERVAL);
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
