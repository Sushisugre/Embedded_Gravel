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

#define INTERVAL 200

extern void sleep(unsigned long millis);

int main(int argc, char** argv) {
  
  printf("\\");
  fflush(stdout);
  sleep(INTERVAL);

  while(1) {
    printf("\b \b|");
    fflush(stdout);
    sleep(INTERVAL);

    printf("\b \b/");
    fflush(stdout);
    sleep(INTERVAL);

    printf("\b \b-");
    fflush(stdout);
    sleep(INTERVAL);

    printf("\b \b\\");
    fflush(stdout);
    sleep(INTERVAL);
  }

  return 0;
}
