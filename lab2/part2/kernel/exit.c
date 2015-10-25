/*
 * exit.c:
 *
 * Author: Shi Su <shis@andrew.cmu.edu>
 *
 * Date:   The current time & date
 */

 // jump to the clean up and return section of call_user
 // will return to kernel with r0 set to status
 extern void exit_user(int status);

 void exit(int status){
    exit_user(status);
 }


