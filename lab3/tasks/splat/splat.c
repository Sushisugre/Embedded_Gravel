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

int main(int argc, char** argv) {
  
  while(1) {
    puts("|");
    sleep(INTERVAL);
    puts("\b \b/");
    sleep(INTERVAL);
    puts("\b \b-");
    sleep(INTERVAL);
    puts("\b \b//");
    sleep(INTERVAL);
  }

  return 0;
}
