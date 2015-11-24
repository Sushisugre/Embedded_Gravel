/** @file crt0.S
 *
 * @brief Execution startup routine
 *
 * @author Shi Su <shis@andrew.cmu.edu>
 * @author Mengjing Yan <mengjinyan@cmu.edu>
 * 
 * @date   Tue Nov 24 09:03:50 EST 2015
 */

#include <asm.h>

	.file "crt0.S"

FUNC(_start)
@ Load value argc from stack, and move the stack pointer 4 byte
	ldr r0, [sp], #4 
    @ Send address of the sp(argv) to r1
    mov r1, sp 
     @ Call main function      
    bl main 
    @ Infinite loop    
.Loop   
    subs r1, r1, r1
    beq .Loop  
