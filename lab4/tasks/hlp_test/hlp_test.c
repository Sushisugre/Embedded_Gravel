/** @file hlp_test.c
 *
 * @brief Create three tasks. 
 *        Task 0 have the lowest native priority.
 *        Task 1 have the medium native priority.
 *        Task 2 have the highest native priority.
 *        Task 1 and Task 2 are simple periodic tasks.
 *        Task 0 first hold mutex for 3s and then change to perodic task.
 *        
 *        Should print: 21[000...000]21...
 *        Tests: If the current priority of the task which holds a mutex
 *               can be set to 0, and when unlock, can be set to native 
 *               priority.
 *
 * @author Mengjin Yan <myan1@andrew.cmu.edu>
 * @date   2015-12-08
 */

#define HLP
#include <stdio.h>
#include <unistd.h>
#include <task.h>
#include <lock.h>

volatile int mid = -1;

void panic(const char* str)
{
  puts(str);
  while(1);
}

void fun1(void* str)
{
  mid = mutex_create();
  
  mutex_lock(mid);
  
  while(time() <= 300) {
    putchar((int)str);
  }
  
  mutex_unlock(mid);
  
  while(1) {
    putchar((int)str);
    if (event_wait(1) < 0)
      panic("Dev 1 failed");
  }
}

void fun2(void* str)
{
  while(1) {
    putchar((int)str);
    if (event_wait(0) < 0)
      panic("Dev 0 failed");
  }
}

void fun3(void* str)
{
  while(1) {
    putchar((int)str);
    if (event_wait(3) < 0)
      panic("Dev 3 failed");
  }
}

int main(int argc, char** argv)
{
  task_t tasks[3];
  tasks[0].lambda = fun1;
  tasks[0].data = (void*)'0';
  tasks[0].stack_pos = (void*)0xa2000000;
  tasks[0].C = 1;
  tasks[0].T = PERIOD_DEV1;
  tasks[0].B = 0;
  
  tasks[1].lambda = fun2;
  tasks[1].data = (void*)'1';
  tasks[1].stack_pos = (void*)0xa1000000;
  tasks[1].C = 1;
  tasks[1].T = PERIOD_DEV0;
  tasks[1].B = 0;

  tasks[2].lambda = fun3;
  tasks[2].data = (void*)'2';
  tasks[2].stack_pos = (void*)0xa1200000;
  tasks[2].C = 1;
  tasks[2].T = PERIOD_DEV3;
  tasks[2].B = 0;

  task_create(tasks, 3);

  puts("Hmmmmmmmmmm!\n");
  argc = argc;
  argv = argv;
  return 0;
}
