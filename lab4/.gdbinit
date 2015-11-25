set architecture armv5te
target remote localhost:1234
add-symbol-file kernel/kernel 0xa3000000
add-symbol-file ../lab3/tasks/bin/typo 0xa2000000
b read
b read.S:21
b irq_wrapper
b io.c:70
b swi_handler
