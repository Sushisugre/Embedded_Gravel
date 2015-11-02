@ exit.S: exit sycall wrapper
@
@ Authors: Mengjin Yan <myan1@andrew.cmu.edu>
@ Date:    Mon 12 Oct 2015 08:21:57 PM EDT

#include <bits/swi.h>
	
	.file	"exit.S"
	.text

	.global	exit
exit:
	@ Exit with status in r0
	@ Do not need to change convertion
	@ Since the system call is void, no need to check the return value
	swi	EXIT_SWI
