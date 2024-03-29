/*
 * time_driver.c: time_driver to handle irq interrupt 
 *
 * Author: Mengjin Yan <mengjinyan@cmu.edu>
 *         Shi Su <shis@andrew.cmu.edu>
 * 
 * Date: Thu Nov 5 23:48:35 EST 2015
 */

#include <arm/reg.h>
#include <arm/timer.h>
#include <sched.h>
#include <device.h>

 /**
 * System time since kernel init in milisecond
 */
extern unsigned g_ms_counter; 

void time_driver() {

  // Set the counter register to zero
  reg_write(OSTMR_OSCR_ADDR, 0x0);
  
  // Clear OSSR
  reg_set(OSTMR_OSSR_ADDR, OSTMR_OSSR_M0);

  // Update the counters
  g_ms_counter = g_ms_counter + 10;

  // check each device to see if there're tasks to wake up
  dev_update(g_ms_counter);
}
