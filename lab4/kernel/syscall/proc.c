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

    // check task number
    // reserve the lowest priority task for idle
    // reserve the highest priority for part 2
    // only 62 can be real tasks
    if (num_tasks <=0 || num_tasks > OS_AVAIL_TASKS - 1)
    {
        return -EINVAL;
    }

    // check tasks stay in valid address
    if(!valid_addr((void *)tasks, num_tasks * sizeof(task_t), 
                USR_START_ADDR, USR_END_ADDR)){
        return -EFAULT;
    }

    // array of task pointers
    task_t* task_ptrs[num_tasks];
    int i;
    for (i = 0; i < (int)num_tasks; ++i)
    {
        // check tasks user stack in valid address, has correct aligment
        if(!(valid_addr(tasks[i].stack_pos,
                (size_t)OS_USTACK_SIZE,
                USR_START_ADDR, USR_END_ADDR))
            || tasks[i].stack_pos % OS_USTACK_ALIGN != 0) {
                return -EFAULT;
        }


        task_ptrs[i] = &tasks[i];
    }
    // check schedulable, 
    // The task list at the end of this method will be sorted in order is priority
    if (!assign_schedule(task_ptrs, num_tasks)){
        return -ESCHED;
    }

    disable_interrupts();
    allocate_tasks(task_ptrs, num_tasks);

    /**
     * code should not come here
     */
    return 1; 
}

int event_wait(unsigned int dev  __attribute__((unused)))
{
    // invalid device num
    if (dev >= NUM_DEVICES){
        return -EINVAL;
    }

    // return a EHOLDSLOCK error if a task calls dev wait while holding a lock1
    // if (get_cur_tcb()->holds_lock){
    //     return -EHOLDSLOCK;
    // }

    // get the tcb of the current task
    dev_wait(dev);

    return 1; 	
}

/* An invalid syscall causes the kernel to exit. */
void invalid_syscall(unsigned int call_num  __attribute__((unused)))
{
	printf("Kernel panic: invalid syscall -- 0x%08x\n", call_num);

	disable_interrupts();
	while(1);
}
