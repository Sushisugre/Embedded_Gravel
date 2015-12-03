/** @file run_queue.c
 * 
 * @brief Run queue maintainence routines.
 *
 * @author Kartik Subramanian <ksubrama@andrew.cmu.edu>
 * @date 2008-11-21
 *
 * @author Shi Su <shis@andrew.cmu.edu>
 *         Mengjin Yan <mengjinyan@cmu.edu>  
 *     
 * @date Thu Dec  3 00:14:22 EST 2015
 * 
 */

#include <types.h>
#include <assert.h>

#include <kernel.h>
#include <sched.h>
#include "sched_i.h"



static tcb_t* run_list[OS_MAX_TASKS]  __attribute__((unused));

/* A high bit in this bitmap means that the task whose priority is
 * equal to the bit number of the high bit is runnable.
 */
static uint8_t run_bits[OS_MAX_TASKS/8] __attribute__((unused));

/* This is a trie structure.  Tasks are grouped in groups of 8.  If any task
 * in a particular group is runnable, the corresponding group flag is set.
 * Since we can only have 64 possible tasks, a single byte can represent the
 * run bits of all 8 groups.
 */
static uint8_t group_run_bits __attribute__((unused));

/* This unmap table finds the bit position of the lowest bit in a given byte
 * Useful for doing reverse lookup.
 */
static uint8_t prio_unmap_table[]  __attribute__((unused)) =
{

0, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
5, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
6, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
5, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
7, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
5, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
6, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
5, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0
};

/**
 * @brief Clears the run-queues and sets them all to empty.
 */
void runqueue_init(void)
{
	int i;
	
	// clear run_list
	for(i = 0; i < OS_MAX_TASKS; i++)
		run_list[i] = 0;

	// clear run_bits
	for(i = 0; i < (OS_MAX_TASKS/8); i++)
		run_bits[i] = 0;

	// clear gourp_run_bits
	group_run_bits = 0;
}

/**
 * @brief Adds the thread identified by the given TCB to the runqueue at
 * a given priority.
 *
 * The native priority of the thread need not be the specified priority.  The
 * only requirement is that the run queue for that priority is empty.  This
 * function needs to be externally synchronized.
 */
void runqueue_add(tcb_t* tcb  __attribute__((unused)), uint8_t prio  __attribute__((unused))) {
	uint8_t OSTCBY, OSTCBX;

	// set prio in tcb
	tcb->cur_prio = prio;

	// put the task into run_queue
	run_list[prio] = tcb;

	// compute priority group (OSTCBY field) of the task
	OSTCBY = prio >> 3;

	// compute task'sposition in the priority group (OSTCBX field)
	OSTCBX = prio & 0x07;

	// set bit number OSTCBY of group_run_bits to 1
	group_run_bits = group_run_bits | (0x01 << (OSTCBY));

	// set bit number OSTBX of run_bits[OSTCBY] equal to 1
	run_bits[OSTCBY] = run_bits[OSTCBY] | (0x01 << (OSTCBX));
}


/**
 * @brief Empty the run queue of the given priority.
 *
 * @return  The tcb at enqueued at the given priority.
 *
 * This function needs to be externally synchronized.
 */
tcb_t* runqueue_remove(uint8_t prio  __attribute__((unused)))
{
	tcb_t* remove_task;
	uint8_t OSTCBX, OSTCBY;

	// remove task from run_list
	remove_task = run_list[prio];
	run_list[prio] = 0;

	// compute OSTCBY and OSTCBX
	OSTCBX = prio & 0x07;
	OSTCBY = prio >> 3;

	// set bit number OSTCBX of run_bits[OSTCBY] equal to 0
	run_bits[OSTCBY] = run_bits[OSTCBY] ^ (0x01 << (OSTCBX));

	// set bit number OSTBY of group_run_bits to 0 if necessary
	if(run_bits[OSTCBY] == 0) {
		group_run_bits = group_run_bits ^ (0x01 << (OSTCBY));
	}

	return remove_task; 
}

/**
 * @brief This function examines the run bits and the run queue and returns the
 * priority of the runnable task with the highest priority (lower number).
 */
uint8_t highest_prio(void)
{
	// find the least significant bit set in group_bits
	uint8_t y = prio_unmap_table[group_run_bits];

	// find the least significant bit set in run_bits[y]
    uint8_t x = prio_unmap_table[run_bits[y]];
    
    return (y << 3) + x;
}
