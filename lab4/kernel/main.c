/** @file main.c
 *
 * @brief kernel main
 *
 * @author Shi Su <shis@andrew.cmu.edu>
 *         Mengjin Yan <mengjinyan@cmu.edu>  
 *	   
 * @date   
 */
 
#include <kernel.h>
#include <task.h>
#include <sched.h>
#include <device.h>
#include <assert.h>
#include <config.h>
#include <arm/timer.h>
#include <arm/interrupt.h>
#include <arm/reg.h>
#include <lock.h>



#define WORD 4
#define LDR_BASE 0xe59ff000
#define LDR_PC_MINUS_4 0xe51ff004
#define LDR_MASK 0xfffff000
#define SWI_VECTOR 0x08
#define IRQ_VECTOR 0x18
#define E_BADCODE 0x0badc0de

uint32_t global_data;

/**
 * Saved svc stack pointer to data section
 */
uint32_t g_svc_stack = 0;

/**
 * System time since kernel init in milisecond
 */
extern unsigned g_ms_counter; 


/**
 * swi handler in assembly
 * get the swi num then transfer the control to c_swi_handler
 */
extern void swi_handler(unsigned swi_num);

/**
 * IRQ wrapper which move irq handling from irq mode to svc mode 
 */
extern void irq_wrapper();

/**
 * Set IRQ stack
 */
extern void set_irq_stack();


void install_handler(unsigned *old_handler, unsigned *new_handler);
void restore_handler(unsigned *old_handler, unsigned *old_inst);
unsigned* get_old_handler(unsigned* vector);
void update_interrupt_controller(unsigned icmr, unsigned iclr);
void init_timer();

int kmain(int argc __attribute__((unused)), char** argv  __attribute__((unused)), uint32_t table)
{

	app_startup();
	global_data = table;
	/* add your code up to assert statement */

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
    install_handler(old_irq_handler, (unsigned*)&irq_wrapper);

    unsigned old_icmr = reg_read(INT_ICMR_ADDR);
    unsigned old_iclr = reg_read(INT_ICLR_ADDR);

    // mask all devices except OSMR0 in ICMR
    // set OSMR0 to generate IRQ in ICLR
    // other devices are masked so the value in ICLR has no effect on them
    update_interrupt_controller(1 << INT_OSTMR_0, 0);
    
    // init mutex here for mutex may be created before any tasks
    mutex_init();
    // dev_init();

    init_timer();

    // setup for usermode & call user program
    enter_user_mode();

/*********** The following code will not be executed *******************/
/**************** As Gravelv2 will not return ************************8*/
  
    // restore native swi/irq handler, interrupt controller
    restore_handler(old_swi_handler, old_swi_inst);
    restore_handler(old_irq_handler, old_irq_inst);
    update_interrupt_controller(old_icmr, old_iclr);

	assert(0);        /* should never get here */
}


/**
 * clear OSCR
 * clear global time counters
 * mask timer interrupt in OIER
 * set OSMR0 to generate interrup every 10ms
 */
void init_timer() {

    // clear global counters
    g_ms_counter = 0;

    // Clear OSSR
    reg_set(OSTMR_OSSR_ADDR, OSTMR_OSSR_M0);
    // enable OSMR0 in OIER
    reg_write(OSTMR_OIER_ADDR, OSTMR_OIER_E0);
    // set the value of OS Timer Counter Register to 0
    reg_write(OSTMR_OSCR_ADDR, 0);
    // set the value of OS Timer Counter Register to 10 milisecond
    reg_write(OSTMR_OSMR_ADDR(0), OS_TIMER_INTERVAL);
}

/**
 * Mask interrupt in interrupt controler, and decide the level of interrupt
 * @param icmr value of icmr
 * @param iclr value of iclr
 */
void update_interrupt_controller(unsigned icmr, unsigned iclr) {
    reg_write(INT_ICMR_ADDR, icmr);
    reg_write(INT_ICLR_ADDR, iclr);
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
