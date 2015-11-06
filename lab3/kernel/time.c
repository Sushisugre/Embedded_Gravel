/*
 * time.c: time syscall
 *
 * Author: Shi Su <shis@andrew.cmu.edu>
 *         Mengjin Yan <mengjinyan@cmu.edu>
 *         
 * Date:  Fri Nov  6 00:21:22 EST 2015
 */

// milisecond
extern unsigned g_ms_counter; 
// second
extern unsigned g_s_counter;
// minute
extern unsigned g_m_counter;
// hour
extern unsigned g_h_counter;

/**
 * Get the time in milliseconds that have elapsed since the kernel booted up.
 * @return time 
 */
unsigned long time(){

    return 3600000 * g_h_counter 
         + 60000 * g_m_counter
         + 1000 * g_s_counter
         + g_ms_counter;

}
