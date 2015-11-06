/*
 * sleep.c: sleep syscall
 *
 * Author: Shi Su <shis@andrew.cmu.edu>
 *         Mengjin Yan <mengjinyan@cmu.edu>
 *         
 * Date:  
 */

extern unsigned long time();

/**
 * Suspends the execution of the current task for the given time.
 * @param millis sleep time
 */
void sleep(unsigned long millis){

    unsigned long start = time();
    unsigned long end = start + millis;

    while(time() < end);
}
