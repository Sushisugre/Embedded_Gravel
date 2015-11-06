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

#define TEN_MILLION_SECONDS 32500

extern unsigned g_ms_counter;
extern unsigned g_s_counter;
extern unsigned g_m_counter;
extern unsigned g_h_counter;

void time_driver() {
  
  // Set the counter register to zero
  reg_write(OSTMR_OSCR_ADDR, 0x0);
  
  // Clear OSSR
  reg_clear(OSTMR_OSSR_ADDR, OSTMR_OSSR_M0);

  // Update the counters
  g_ms_counter = g_ms_counter + 10;
  
  if(g_ms_counter >= 1000) {
      g_ms_counter = 0;
      g_s_counter = g_s_counter + 1;

      if(g_s_counter >= 60) {
          g_s_counter = 0;
          g_m_counter = g_m_counter + 1;

      if(g_m_counter >= 60) {
          g_m_counter = 0;
	        g_h_counter = g_h_counter + 1;
      }
    }
  }
}
