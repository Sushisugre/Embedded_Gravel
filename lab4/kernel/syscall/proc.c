/** @file proc.c
 * 
 * @brief Implementation of `process' syscalls
 *
 * @author Mike Kasick <mkasick@andrew.cmu.edu>
 * @date   Sun, 14 Oct 2007 00:07:38 -0400
 *
 * @author Kartik Subramanian <ksubrama@andrew.cmu.edu>
 * @date 2008-11-12
 */

#include <exports.h>
#include <bits/errno.h>
#include <config.h>
#include <kernel.h>
#include <syscall.h>
#include <sched.h>

#include <arm/reg.h>
#include <arm/psr.h>
#include <arm/exception.h>
#include <arm/physmem.h>
#include <device.h>

int task_create(task_t* tasks  __attribute__((unused)), size_t num_tasks  __attribute__((unused)))
{

    disable_interrupts();

    // check task number
    if (num_tasks <=0 || num_tasks > OS_AVAIL_TASKS)
    {
        enable_interrupts();
        return -EINVAL;
    }

    // check schedulable, 
    // The task list at the end of this method will be sorted in order is priority
    // TODO: update assign_schedule in part2, now it's just a dummy
    if (assign_schedule(&tasks, num_tasks)){
        enable_interrupts();
        return -ESCHED;
    }

    allocate_tasks(&tasks, num_tasks);

    /**
     * code should not come here
     */
    return 1; 
}

int event_wait(unsigned int dev  __attribute__((unused)))
{
  return 1; /* remove this line after adding your code */	
}

/* An invalid syscall causes the kernel to exit. */
void invalid_syscall(unsigned int call_num  __attribute__((unused)))
{
	printf("Kernel panic: invalid syscall -- 0x%08x\n", call_num);

	disable_interrupts();
	while(1);
}
