/** @file ub_test.c
 * 
 * @brief The UB Test for basic schedulability
 *
 * @author Kartik Subramanian <ksubrama@andrew.cmu.edu>
 * @date 2008-11-20
 */

//#define DEBUG 0

#include <sched.h>
#ifdef DEBUG
#include <exports.h>
#endif

/**
 * the array to store the u(n) value of the ub test
 * for better performance
 */ 
static float u_n_num[64] = {
	1.000, 0.828, 0.780, 0.757, 0.743, 0.735, 0.729, 0.724,
	0.721, 0.718, 0.715, 0.714, 0.712, 0.711, 0.709, 0.708, 
	0.707, 0.707, 0.706, 0.705, 0.705, 0.704, 0.704, 0.703, 
	0.703, 0.702, 0.702, 0.702, 0.701, 0.701, 0.701, 0.701, 
	0.700, 0.700, 0.700, 0.700, 0.700, 0.700, 0.699, 0.699, 
	0.699, 0.699, 0.699, 0.699, 0.699, 0.698, 0.698, 0.698, 
	0.698, 0.698, 0.698, 0.698, 0.698, 0.698, 0.698, 0.697, 
	0.697, 0.697, 0.697, 0.697, 0.697, 0.697, 0.697, 0.697
};

/**
 * Sort the task list in order is priority
 * @param tasks An array of task pointers containing the task set to schedule.
 */
void sort_tasks(task_t** tasks  __attribute__((unused)), size_t num_tasks  __attribute__((unused))) {
    size_t i, j, k;
    task_t* temp;

    // insertion sort
    for (i = 0; i < num_tasks; i++) {
    	for(j = 0; j < i; j++) {
    		if((tasks[i]->T) < (tasks[j]->T)) {
    			temp = tasks[i];
    			for(k = i-1; k >= j; k--) {
    				tasks[k+1] = tasks[k];
    			}
    			tasks[j] = temp;
    			break;
    		}
    	}
    }
}

/**
 * @brief Perform UB Test and reorder the task list.
 *
 * The task list at the end of this method will be sorted in order is priority
 * -- from highest priority (lowest priority number) to lowest priority
 * (highest priority number).
 *
 * @param tasks  An array of task pointers containing the task set to schedule.
 * @param num_tasks  The number of tasks in the array.
 *
 * @return 0  The test failed.
 * @return 1  Test succeeded.  The tasks are now in order.
 */
int assign_schedule(task_t** tasks  __attribute__((unused)), size_t num_tasks  __attribute__((unused)))
{
	size_t i;
	float result = 0;

	// do ub test
	// calculate the result on the left side of the equation
	for(i = 0; i < num_tasks; i++) {
        task_t task = ((task_t*)tasks)[i];
		result = result + ((float)(task.C))/((float)(task.T));
	}

	// check the value
	if(result <= u_n_num[num_tasks-1]) {
		// if pass ub test, then sort the tasks according to the priority
		sort_tasks(tasks, num_tasks);
		return 1;
	} else {
		// if not pass ub test
		return 0;
	}

	//return 1; // fix this; dummy return to prevent compiler warnings	
}
