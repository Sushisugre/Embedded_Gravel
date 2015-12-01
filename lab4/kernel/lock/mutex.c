/**
 * @file mutex.c
 *
 * @brief Implements mutexes.
 *
 * @author Mengjin Yan <myan1@andrew.cmu.edu>
 *         Shi Su <shis@andrew.cmu.edu>
 * 
 * @date  
 */

//#define DEBUG_MUTEX

#include <lock.h>
#include <task.h>
#include <sched.h>
#include <bits/errno.h>
#include <arm/psr.h>
#include <arm/exception.h>
#include <config.h>
#ifdef DEBUG_MUTEX
#include <exports.h> // temp
#endif

mutex_t gtMutex[OS_NUM_MUTEX];

void mutex_init()
{
    int i;
    for (i = 0; i < OS_NUM_MUTEX; i++)
    {
        gtMutex[i].bAvailable = TRUE;
        gtMutex[i].pHolding_Tcb = 0;
        gtMutex[i].bLock = 0;
        gtMutex[i].pSleep_queue = 0;
    }
}

int mutex_create(void)
{
	int i;

    // allocate an available mutex to the tcb
    for(i = 0; i < OS_NUM_MUTEX; i++) {
        if(gtMutex[i].bAvailable == TRUE) {
            gtMutex[i].bAvailable = FALSE;
            return i;
        }
    }

    // if there is no available mutex, return error
	return -ENOMEM; 
}

int mutex_lock(int mutex  __attribute__((unused)))
{
    tcb_t* temp;
    
    disable_interrupts();

    // if the provided mutex identifier if invalid
    if(mutex > OS_NUM_MUTEX) {
        
        enable_interrupts();
        return -EINVAL;
    
    } 

    if(gtMutex[mutex].pHolding_Tcb == get_cur_tcb()) {

        // if the current task is already holding the lock
        enable_interrupts();
        return -EDEADLOCK;

    } else {
        if(gtMutex[mutex].bLock == 0) {
            // if the lock is not been locked
            // set lock status and holding tcb
            gtMutex[mutex].bLock = 1;
            gtMutex[mutex].pHolding_Tcb = get_cur_tcb();
            enable_interrupts();
            return 0;
        } else {
            // if the lock is been locked
            // move the current tack to mutex sleeping queue
            temp = gtMutex[mutex].pSleep_queue;
            while((temp->sleep_queue) != 0) {
                temp = temp->sleep_queue;
            }
            temp->sleep_queue = get_cur_tcb();

            // context switch to another task
            dispatch_save();
        }
    }

    // the function cannot get here
    return 1; // fix this to return the correct value
}

int mutex_unlock(int mutex  __attribute__((unused)))
{
    tcb_t* temp;

    disable_interrupts();

    // if the provided mutex identifier if invalid
    if(mutex > OS_NUM_MUTEX) {
        
        enable_interrupts();
        return -EINVAL;
    
    } 

    if(gtMutex[mutex].pHolding_Tcb != get_cur_tcb()) {
        
        // if the urrent task does not hold the mutex
        enable_interrupts();
        return -EPERM;
    
    } else {
        temp = gtMutex[mutex].pHolding_Tcb;

        // if there is no element in sleep queue
        if(gtMutex[mutex].pSleep_queue == 0) {
            gtMutex[mutex].bLock = 0;
            gtMutex[mutex].pHolding_Tcb = 0;
            enable_interrupts();
            return 0;
        } else {
            // if the sleep queue have tasks waiting for the mutex
            // set mutex_t
            gtMutex[mutex].pHolding_Tcb = gtMutex[mutex].pSleep_queue;
            gtMutex[mutex].pSleep_queue = gtMutex[mutex].pSleep_queue->sleep_queue;

            // put the wake up task into runqueue
            runqueue_add(gtMutex[mutex].pHolding_Tcb, gtMutex[mutex].pHolding_Tcb->cur_prio);
        
            // check the wake up task's priority and current running task's priority
            if(highest_prio() < get_cur_prio()) {
                // switch to the highest task
                dispatch_save();
            } else {
                // stay the same
                enable_interrupts();
                return 0;
            }
        }
    }

    // the function will not get here
	return 1; // fix this to return the correct value
}

