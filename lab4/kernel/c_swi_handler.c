/*
 * c_swi_handler.c: swi handler in c, call libc according to swi number
 *
 * Author: Shi Su <shis@andrew.cmu.edu>
 *         Mengjin Yan <mengjinyan@cmu.edu>
 *
 * Date:   Mon Oct 26 14:03:58 EDT 2015
 */


#include <bits/swi.h>
#include <types.h>
#include <syscall.h>
#include <lock.h>

/* Dispatch the syscall to different lib function according to swi_num
 swi number and pointer to the register value on stack is passed from assembly swi handler */
void c_swi_handler(unsigned swi_num, unsigned* regs){


    switch(swi_num){
        case READ_SWI:
        {
            // cast unsigned address to buffer pointer
            char *buf = (char*)regs[1];
            // call lib and put the return value back on stack
            regs[0] = read_syscall(regs[0], buf, regs[2]);
            break;
        }
        case WRITE_SWI:
        {
            char *buf = (char*)regs[1];
            regs[0] = write_syscall(regs[0], buf, regs[2]);
            break;
        }
        case TIME_SWI:
        {
            // unsigned long to long?
            regs[0] = (unsigned) time_syscall();
            break;
        }
        case SLEEP_SWI:
        {
            sleep_syscall(regs[0]);
            break;
        }
        case CREATE_SWI:
        {
            regs[0] = task_create((task_t*)regs[0], regs[1]);
            break;
        }
        case MUTEX_CREATE:
        {
            regs[0] = mutex_create();
            break;
        }
        case MUTEX_LOCK:
        {
            regs[0] = mutex_lock(regs[0]);
            break;
        }
        case MUTEX_UNLOCK:
        {
            regs[0] = mutex_unlock(regs[0]);
            break;
        }
        case EVENT_WAIT:
        {
            regs[0] = event_wait(regs[0]);
            break;
        }        
        default:
            invalid_syscall(regs[0]);

    }
}
