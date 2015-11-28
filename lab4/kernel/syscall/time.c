/** @file time.c
 *
 * @brief Kernel timer based syscall implementations
 *
 * @author Shi Su <shis@andrew.cmu.edu>
 *         Mengjin Yan <mengjinyan@cmu.edu>
 *         
 * @date 2015-11-24
 */

#include <types.h>
#include <config.h>
#include <bits/errno.h>
#include <arm/timer.h>
#include <syscall.h>


/**
 * System time since kernel init in milisecond
 */
volatile unsigned g_ms_counter; 

/**
 * Get the time in milliseconds that have elapsed since the kernel booted up.
 * @return time 
 */
unsigned long time_syscall(void)
{
    return g_ms_counter;
}


// FIXME: switch to wait for event
/** @brief Waits in a tight loop for atleast the given number of milliseconds.
 *
 * @param millis  The number of milliseconds to sleep. * 
 */
void sleep_syscall(unsigned long millis  __attribute__((unused)))
{
	unsigned long start = time_syscall();
    unsigned long end = start + millis;

    // as we don't have other process to switch to
    // just looping during sleep period
    while(time_syscall() < end);
}
