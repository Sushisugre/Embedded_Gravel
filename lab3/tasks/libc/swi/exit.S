/** @file exit.S
 *
 * @brief exit sycall wrapper
 *
 * @author Shi Su <shis@andrew.cmu.edu>
 *         Mengjin Yan <mengjinyan@cmu.edu>
 *         
 * @date Thu Nov  5 23:08:44 EST 2015
 * 
 */

#include <asm.h>
#include <bits/swi.h>

    	.file "exit.S"

        .global exit
FUNC(exit)
        @ r0 is already set by calling program
        swi    EXIT_SWI  
        @ exit has no return value
