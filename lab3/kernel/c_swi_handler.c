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

#define E_BADCODE 0x0badc0de

extern ssize_t write(int fd, const void *buf, size_t count);
extern ssize_t read(int fd, void *buf, size_t count);
extern void exit(int status);

/* Dispatch the syscall to different lib function according to swi_num
 swi number and pointer to the register value on stack is passed from assembly swi handler */
void c_swi_handler(unsigned swi_num, unsigned* regs){


    switch(swi_num){
        case EXIT_SWI:
        {
            exit(regs[0]);
            break;
        }
        case READ_SWI:
        {
            // cast unsigned address to buffer pointer
            char *buf = (char*)regs[1];
            // call lib and put the return value back on stack
            regs[0] = read(regs[0], buf, regs[2]);
            break;
        }
        case WRITE_SWI:
        {
            char *buf = (char*)regs[1];
            regs[0] = write(regs[0], buf, regs[2]);
            break;
        }
        default:
            exit(E_BADCODE);

    }
}
