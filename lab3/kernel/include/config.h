/** @file config.h
 *
 * @brief A set of common OS configuration flags.  This is to control overall
 * OS behavior and not behavior of a particular modules.
 *
 * @author Kartik Subramanian <ksubrama@andrew.cmu.edu>
 * @date 2008-10-28
 */

#ifndef _CONFIG_H_
#define _CONFIG_H_

#define OS_TICKS_PER_SEC        100    /* Set the number of ticks in one second */
#define IRQ_STACK_HEIGHT        100
#define OS_TIMER_INTERVAL       32500 /* 32500 cycle = 10ms */

#endif /* _CONFIG_H_ */
