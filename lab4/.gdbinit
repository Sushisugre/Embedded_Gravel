set architecture armv5te
target remote localhost:1234
add-symbol-file kernel/kernel 0xa3000000
add-symbol-file ../lab4/tasks/bin/dagger 0xa0000000
b task_create
b allocate_tasks
b dispatch_init
b dispatch_nosave
b ctx_switch_half
