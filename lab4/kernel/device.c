/**
 * @file device.c
 *
 * @brief Implements simulated devices.
 * @author Kartik Subramanian <ksubrama@andrew.cmu.edu>
 * @date 2008-12-01
 */

#include <types.h>
#include <assert.h>

#include <task.h>
#include <sched.h>
#include <device.h>
#include <arm/reg.h>
#include <arm/psr.h>
#include <arm/exception.h>
#include <arm/timer.h>

/**
 * @brief Fake device maintainence structure.
 * Since our tasks are periodic, we can represent 
 * tasks with logical devices. 
 * These logical devices should be signalled periodically 
 * so that you can instantiate a new job every time period.
 * Devices are signaled by calling dev_update 
 * on every timer interrupt. In dev_update check if it is 
 * time to create a tasks new job. If so, make the task runnable.
 * There is a wait queue for every device which contains the tcbs of
 * all tasks waiting on the device event to occur.
 */

/**
 * System time since kernel init in milisecond
 */
extern unsigned g_ms_counter; 

struct dev
{
	tcb_t* sleep_queue;
	unsigned long   next_match;
};
typedef struct dev dev_t;

/* devices will be periodically signaled at the following frequencies */
const unsigned long dev_freq[NUM_DEVICES] = {100, 200, 500, 50};
static dev_t devices[NUM_DEVICES];

/**
 * @brief Initialize the sleep queues and match values for all devices.
 */
void dev_init(void)
{
    int i;
   for (i = 0; i < NUM_DEVICES; i++)
   {
       devices[i].sleep_queue = 0;
       // call dev_init before timer init
       devices[i].next_match = g_ms_counter + dev_freq[i];
   }
}


/**
 * @brief Puts a task to sleep on the sleep queue until the next
 * event is signalled for the device.
 *
 * @param dev  Device number.
 */
void dev_wait(unsigned int dev __attribute__((unused)))
{
	tcb_t* current_tcb;

	// get the tcb of the current task
	current_tcb = get_cur_tcb();

	// put the task in corresponding sleep queue
	devices[dev].sleep_queue = current_tcb;

    disable_interrupts();
    dispatch_sleep();
}


/**
 * @brief Signals the occurrence of an event on all applicable devices. 
 * This function should be called on timer interrupts to determine that 
 * the interrupt corresponds to the event frequency of a device. If the 
 * interrupt corresponded to the interrupt frequency of a device, this 
 * function should ensure that the task is made ready to run 
 */
void dev_update(unsigned long millis __attribute__((unused)))
{
	int i;

	// check if the task needs to be waked up
	for(i = 0; i < NUM_DEVICES; i++) {
		if(devices[i].next_match <= millis) {

			// change the device next match time
			devices[i].next_match = devices[i].next_match + dev_freq[i];

			// make the task ready to runqueue
            if(devices[i].sleep_queue) {

                tcb_t* wake_tcb = devices[i].sleep_queue;
			    runqueue_add(wake_tcb, wake_tcb->cur_prio);
                // drop the task from sleep queue
                devices[i].sleep_queue = 0;

                if (wake_tcb->cur_prio < get_cur_prio()) {
                    dispatch_save();
                }
            }

		}
	}
}

