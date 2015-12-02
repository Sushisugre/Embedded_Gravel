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
#include <device.h>
#include <lock.h>

tcb_t system_tcb[OS_MAX_TASKS]; /*allocate memory for system TCBs */

extern mutex_t gtMutex[OS_NUM_MUTEX];

/**
 * This method will be called before allocating new tasks
 * clear previous tasks and their status 
 */
void sched_init(task_t* main_task  __attribute__((unused)))
{
    // don't really understand the parameter 
    // not used here
    
    // clear runbits
    runqueue_init();

    // intitalte devices
    dev_init();

    // clear mutex holding
    int i;
    for (i = 0; i < OS_NUM_MUTEX; i++)
    {
        // don't change the bAvailable flag
        // what is created, is created
        // for the case which mutexes are created before tasks
        gtMutex[i].pHolding_Tcb = 0;
        gtMutex[i].bLock = 0;
        gtMutex[i].pSleep_queue = 0;
    }

}

/**
 * @brief This is the idle task that the system runs when no other task is runnable
 */
 
static void __attribute__((unused)) idle(void)
{
	 enable_interrupts();
	 while(1);
}

/**
 * @brief Given a task, initiate its TCB context
 */
void context_init(task_t* task, tcb_t* tcb) {

    // user entry point, i.e. task function
    tcb->context.r4 = (uint32_t) task->lambda;
    // function argument
    tcb->context.r5 = (uint32_t) task->data;
    // user mode stack, top of the user mode stack
    tcb->context.r6 = (uint32_t) task->stack_pos;
    tcb->context.r7 = 0;
    tcb->context.r8 = global_data;
    tcb->context.r9 = 0;
    tcb->context.r10 = 0;
    tcb->context.r11 = 0;
    // after first ctx_switch_half, branch to launch_task
    tcb->context.lr = &launch_task;
    // initial sp is the high address of kstack in tcb
    tcb->context.sp = (void*)tcb->kstack_high;
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
    sched_init((task_t*)0);

    /**
     * Setup idle tcb
     */
    tcb_t* idle_tcb = &system_tcb[IDLE_PRIO];

    // user entry point, i.e. task function
    idle_tcb->context.r4 = (uint32_t) &idle;
    // function argument
    idle_tcb->context.r5 = 0;
    // user mode stack, top of the user mode stack
    // idle_tcb->context.r6 = (uint32_t) &(g_idle_stack[OS_KSTACK_SIZE/sizeof(uint32_t)]);
    idle_tcb->context.r6 = (uint32_t) USR_STACK;
    idle_tcb->context.r7 = 0;
    idle_tcb->context.r8 = global_data;
    idle_tcb->context.r9 = 0;
    idle_tcb->context.r10 = 0;
    idle_tcb->context.r11 = 0;
    // after first ctx_switch_half, branch to launch_task
    idle_tcb->context.lr = &launch_task;
    // initial sp is the high address of kstack in tcb
    idle_tcb->context.sp = (void*)idle_tcb->kstack_high;

    idle_tcb->native_prio = IDLE_PRIO;
    idle_tcb->cur_prio = IDLE_PRIO;
    idle_tcb->holds_lock = 0;
    idle_tcb->sleep_queue = 0;

    // make idle task run
    disable_interrupts();
    dispatch_init(idle_tcb);
    enable_interrupts();

    /**
     * Setup up passed in tasks
     */
    int i;
    for (i = 0; i < (int)num_tasks; i++)
    {
        // save highest priority 0 for part 2
        uint8_t init_prio = i + 1;
        tcb_t* tcb = &system_tcb[init_prio];

        context_init(tasks[i], tcb);
        // context_init(&((task_t*)tasks)[i], tcb);
        tcb->native_prio = init_prio; 
        tcb->cur_prio = init_prio; 
        tcb->holds_lock = 0;
        tcb->sleep_queue = 0;

        // add the new tasks to ready queue
        runqueue_add(tcb, tcb->cur_prio);
    }

    // context switch to the highest priority task 
    dispatch_nosave();
}

