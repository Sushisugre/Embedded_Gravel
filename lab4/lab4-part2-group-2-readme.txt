18342 Fundumentals of Embedded Systems

Group 2: Shi Su <shis@andrew.cmu.edu>
         Mengjin Yan <mengjinyan@cmu.edu>
Date: 2015.12.08


- Kernel Changes
    Include UB test when trying to assign schedule
    After acquiring mutex, raise the priority of task to 0
    After releasing mutex, set the priority of the task back to native priority
    Update task structure to add worse case blocking time B 
    Event_wait throws exception EHOLDSLOCK when current task holds mutex

- Makefile changes
    The implementation of Highest Locker Priority protocol is surrounding by #ifdef marco
    If "-D HLP" flag is specified in KCFLAGS in Makefile, the kernel follows HLP protocol
    If the flag is not specified, the kernel behaves the same as part 1

- Test case changes
    Assign a value B for each of the task

- New test case: hlp_test
    This test creates three tasks: 
    Task 0 have the lowest native priority.
    Task 1 have the medium native priority.
    Task 2 have the highest native priority.
    Task 1 and Task 2 are simple periodic tasks.
    Task 0 first hold mutex for 3s, then it's raised to highest priority, and keep running for 3s without being preempted. Then it releases mutex and return to its native priority.
        
    Should print: 21[000...000]21...
    Tests: If the current priority of the task which holds a mutex
           can be set to 0, and when unlock, can be set to native 
           priority.

