/** @file ctx_switch.c
 * 
 * @brief C wrappers around assembly context switch routines.
 *
 * @author Shi Su <shis@andrew.cmu.edu>
 *         Mengjin Yan <mengjinyan@cmu.edu>  
 *     
 * @date Thu Dec  3 00:14:22 EST 2015
 */
 

#include <types.h>
#include <assert.h>

#include <config.h>
#include <kernel.h>
#include "sched_i.h"
#include <arm/psr.h>
#include <arm/exception.h>

#ifdef DEBUG_MUTEX
#include <exports.h>
#endif

static __attribute__((unused)) tcb_t* cur_tcb; /* use this if needed */

/**
 * @brief Initialize the current TCB and priority.
 *
 * Set the initialization thread's priority to IDLE so that anything
 * will preempt it when dispatching the first task.
 *
 * This function needs to be externally synchronized.
 */
void dispatch_init(tcb_t* idle __attribute__((unused)))
{
    cur_tcb = idle;
    runqueue_add(cur_tcb, cur_tcb->cur_prio);
}


/**
 * @brief Context switch to the highest priority task while saving off the 
 * current task state.
 *
 * This function needs to be externally synchronized.
 * We could be switching from the idle task.  The priority searcher has been tuned
 * to return IDLE_PRIO for a completely empty run_queue case.
 *
 * This function needs to be externally synchronized.
 */
void dispatch_save(void)
{
	tcb_t* target_tcb = runqueue_remove(highest_prio());
    tcb_t* previous_tcb = cur_tcb;

    // current task is still runable
    runqueue_add(previous_tcb, previous_tcb->cur_prio);

    cur_tcb = target_tcb;

    ctx_switch_full((void*)&(target_tcb->context), (void*)&(previous_tcb->context));

}

/**
 * @brief Context switch to the highest priority task that is not this task -- 
 * don't save the current task state.
 *
 * There is always an idle task to switch to.
 *
 * This function needs to be externally synchronized.
 */
void dispatch_nosave(void)
{
    tcb_t* target_tcb = runqueue_remove(highest_prio());
  
    cur_tcb = target_tcb;

    ctx_switch_half((void*)&(target_tcb->context));
    // launch_task();
}


/**
 * @brief Context switch to the highest priority task that is not this task -- 
 * and save the current task but don't mark is runnable.
 *
 * There is always an idle task to switch to.
 *
* This function needs to be externally synchronized.
 */
void dispatch_sleep(void)
{
    tcb_t* target_tcb = runqueue_remove(highest_prio());
    tcb_t* previous_tcb = cur_tcb;

    cur_tcb = target_tcb;

    ctx_switch_full((void*)&(target_tcb->context), (void*)&(previous_tcb->context));
}

/**
 * @brief Returns the priority value of the current task.
 */
uint8_t get_cur_prio(void)
{
	return cur_tcb->cur_prio; 
}

/**
 * @brief Returns the TCB of the current task.
 */
tcb_t* get_cur_tcb(void)
{
	return cur_tcb; 
}
