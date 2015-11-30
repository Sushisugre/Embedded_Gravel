set architecture armv5te
target remote localhost:1234
add-symbol-file kernel/kernel 0xa3000000
add-symbol-file ../lab4/tasks/bin/dagger 0xa0000000
b dispatch_save
b runqueue_remove
b runqueue_add
b allocate_tasks
b proc.c:task_create
b ctx_switch_half
b ctx_switch_full
b dev_update if millis==100
b assign_schedule
b sort_tasks
