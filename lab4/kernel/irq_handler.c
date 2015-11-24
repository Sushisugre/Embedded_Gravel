/*
 * irq_handler.c: irq handler in c, dispatch according to the device
 *
 * Author: Mengjin Yan <mengjinyan@cmu.edu>
 *         Shi Su <shis@andrew.cmu.edu>
 * 
 * Date: Wed Nov 4 20:42:33 EST 2015  
 */

#include <arm/reg.h>
#include <arm/interrupt.h>

#define TIMER_INTERRUPT 0x04000000

extern void time_driver();

void irq_handler() {
  unsigned icmr_num, icpr_num, iclr_num;
  
  // read the value in register and store in variables
  // to avoid changes in the registers
  icmr_num = reg_read(INT_ICMR_ADDR);
  icpr_num = reg_read(INT_ICPR_ADDR);
  iclr_num = reg_read(INT_ICLR_ADDR);

  // dispatch the interrupt
  if((icmr_num & icpr_num & TIMER_INTERRUPT) != 0x0) {
    time_driver();
  }
}
