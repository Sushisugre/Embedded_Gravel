/*
 * sleep.c: sleep syscall
 *
 * Author: Shi Su <shis@andrew.cmu.edu>
 *         Mengjin Yan <mengjinyan@cmu.edu>
 *         
 * Date:  Fri Nov  6 01:00:01 EST 2015
 */

extern unsigned long time();

/**
 * Suspends the execution of the current task for the given time.
 * @param millis sleep time
 */
void sleep(unsigned long millis){

    unsigned long start = time();
    unsigned long end = start + millis;

    // as we don't have other process to switch to
    // just looping during sleep period
    while(time() < end);
}
