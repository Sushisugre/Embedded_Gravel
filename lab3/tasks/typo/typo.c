/** @file typo.c
 *
 * @brief Echos characters back with timing data.
 *
 * Links to libc.
 *
 * @author Mengjin Yan <mengjinyan@cmu.edu>
 *         Shi Su <shis@andrew.cmu.edu>
 * 
 * @date   Fri Nov  6 02:53:01 EST 2015 
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <bits/fileno.h>

#define BUFFER_SIZE 50

int main(int argc, char** argv)
{
  unsigned char buffer[BUFFER_SIZE];
  int read_size;
  unsigned long start_time, end_time;
  double duration;

  while(1) {
    printf("Type Something and Press Enter:\n");
    start_time = time();
    read_size = read(STDIN_FILENO, buffer, BUFFER_SIZE);    
    end_time = time();
    
    duration = ((double)(end_time - start_time))/1000.0;

    if(write(STDOUT_FILENO, buffer, read_size) < 0)
      exit(1);
    // printf("%.1fs\n", duration);
    printf("%d\n", duration);

    if(read_size < 0)
      exit(1);
  }

  return 0;
}
