/*
 * exit.c: exit syscall
 *
 * Author: Shi Su <shis@andrew.cmu.edu>
 *         Mengjin Yan <mengjinyan@cmu.edu>
 *
 * Date:   Mon Oct 26 14:02:15 EDT 2015
 */

  /* jump to the exit_user section of call_user
    will return to kernel with r0 set to status */
 extern void exit_user(int status);

 void exit(int status){
    exit_user(status);
 }


