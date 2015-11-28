/** @file sched.c
 * 
 * @brief Top level implementation of the scheduler.
 *
 * @author Kartik Subramanian <ksubrama@andrew.cmu.edu>
 * @date 2008-11-20
 */

#include <types.h>
#include <assert.h>

#include <kernel.h>
#include <config.h>
#include "sched_i.h"
#include <sched.h>

#include <arm/reg.h>
#include <arm/psr.h>
#include <arm/exception.h>
#include <arm/physmem.h>

tcb_t system_tcb[OS_MAX_TASKS]; /*allocate memory for system TCBs */

void sched_init(task_t* main_task  __attribute__((unused)))
{
	
}

/**
 * @brief This is the idle task that the system runs when no other task is runnable
 */
 
static void __attribute__((unused)) idle(void)
{
	 enable_interrupts();
	 while(1);
}


void contexts_init() {

}

/**
 * @brief Allocate user-stacks and initializes the kernel contexts of the
 * given threads.
 *
 * This function assumes that:
 * - num_tasks < number of tasks allowed on the system.
 * - the tasks have already been deemed schedulable and have been appropriately
 *   scheduled.  In particular, this means that the task list is sorted in order
 *   of priority -- higher priority tasks come first.
 *
 * @param tasks  A list of scheduled task descriptors.
 * @param size   The number of tasks is the list.
 */
void allocate_tasks(task_t** tasks  __attribute__((unused)), size_t num_tasks  __attribute__((unused)))
{

    /**
     * Setup idle tcb
     */
    tcb_t idle_tcb = system_tcb[IDLE_PRIO];

    // user entry point
    idle_tcb.contexts.r4 = (uint32_t) &idle;
    // function argument
    idle_tcb.contexts.r5 = 0;
    // user mode stack, top of the user mode stack
    idle_tcb.contexts.r6 = (uint32_t) USR_STACK;
    idle_tcb.contexts.r7 = 0;
    idle_tcb.contexts.r8 = global_data;
    idle_tcb.contexts.r9 = 0;
    idle_tcb.contexts.r10 = 0;
    idle_tcb.contexts.r11 = 0;
    // initial return address of the task, however the task never return?
    idle_tcb.contexts.lr = 0;
    // not so sure about this
    idle_tcb.contexts.sp = (void*)idle_tcb.kstack_high;

    idle_tcb.native_prio = IDLE_PRIO;
    idle_tcb.cur_prio = IDLE_PRIO;
    idle_tcb.holds_lock = 0;
    idle_tcb.sleep_queue = 0;

    // make idle task run
    dispatch_init(&idle_tcb);

    /**
     * Setup up passed in tasks
     */
    int i;
    for (int i = 0; i < num_tasks; i++)
    {
        system_tcb[i].native_prio = 0; //FIXME
        system_tcb[i].cur_prio = 0; //FIXME
        system_tcb[i].holds_lock = 0;
        system_tcb[i].context = 0;
        system_tcb[i].sleep_queue = 0;

        // add the new tasks to ready queue
        runqueue_add(system_tcb[i], system_tcb[cur_prio]);
    }

    // context switch to the highest priority task 
    dispatch_nosave();
}

