/** @file splat.c
 *
 * @brief Displays a spinning cursor.
 *
 * Links to libc.
 *
 * @author Mengjin Yan <mengjinyan@cmu.edu>
 *         Shi Su <shis@andrew.cmu.edu>
 * 
 * @date Fri Nov  6 00:20:30 EST 2015 
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <bits/fileno.h>

#define INTERVAL 200

int main(int argc, char** argv) {
  
  printf("\\");
  // fflush(STDOUT_FILENO);
  sleep(INTERVAL);

  while(1) {
    printf("\b \b|");
    // fflush(STDOUT_FILENO);
    sleep(INTERVAL);

    printf("\b \b/");
    // fflush(STDOUT_FILENO);
    sleep(INTERVAL);

    printf("\b \b-");
    // fflush(STDOUT_FILENO);
    sleep(INTERVAL);

    printf("\b \b\\");
    // fflush(STDOUT_FILENO);
    sleep(INTERVAL);
  }

  return 0;
}
