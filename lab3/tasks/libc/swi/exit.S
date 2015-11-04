/** @file exit.S
 *
 * @brief exit sycall wrapper
 *
 * @author Mike Kasick <mkasick@andrew.cmu.edu>
 * @date   Sun, 07 Oct 2007 01:52:31 -0400
 * 
 * @author Kartik Subramanian <ksubrama@andrew.cmu.edu>
 * @date 2008-10-29
 */

#include <asm.h>
#include <bits/swi.h>

	.file "exit.S"

    .global exit
FUNC(exit)
    swi    EXIT_SWI  @ r0 is already set by calling program
                     @ exit has no return value