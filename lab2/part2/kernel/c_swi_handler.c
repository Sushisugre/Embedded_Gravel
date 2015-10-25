#include <bits/swi.h>


// use uboot method to implement?
void c_swi_handler(unsigned swi_num, unsigned* regs){

    switch(swi_num){
        case EXIT_SWI:

            break;
        case READ_SWI:

            break;
        case WRITE_SWI:

            break;
        default:
            // exit with 0x0badc0de
            break;

    }
}
