/*
 * rot13.c: ROT13 cipher test application
 *
 * Authors: Mengjin Yan <myan1@andrew.cmu.edu>
 * Date:    Mon Oct 12 23:56:03 EDT 2015
 */

#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <bits/fileno.h>

#define BUFFER_SIZE 50

/*
 * Function: cypher
 * ----------------
 * cyphers the original string by modifying the orignal string
 *
 * block: the pointer to the address of the buffer which stores the 
 *        orignal string.
 * size:  the size of the orignal string.
 */
void cypher(char* block, int size) {
  int i;

  for(i = 0; i < size; i++) {
    if((block[i] >= 'A')&&(block[i] <= 'M')) {
      block[i] = block[i] + 13;
    } else if((block[i] >= 'N')&&(block[i] <= 'Z')) {
      block[i] = block[i] - 13;
    } else if((block[i] >= 'a')&&(block[i] <= 'm')) {
      block[i] = block[i] + 13;
    } else if((block[i] >= 'n')&&(block[i] <= 'z')) {
      block[i] = block[i] - 13;
    }
  }
}

int main(int argc, char *argv[]) {
  int read_size, i, j;
  char block[BUFFER_SIZE];
  char space, newline;

  // Print the argument the main function
  space = ' ';
  newline = '\n';
  for(i = 0; i < argc; i++) {
    for(j = 0; argv[i][j] != '\0'; j++) {
      if(write(STDOUT_FILENO, &(argv[i][j]), 1) < 0)
	exit(1);
    }
    if(write(STDOUT_FILENO, &space, 1) < 0)
      exit(1);
  }

  if(write(STDOUT_FILENO, &newline, 1) < 0)
  exit(1);
  
  // Start to handle input strings
  while(1) {
    read_size = read(STDIN_FILENO, block, BUFFER_SIZE);
    
    if(read_size < 0)
      exit(1);
    
    if(read_size == 0)
      exit(0);
    
    /* When the input is longer than the buffer size, then first fill up
     * the buffer, cypher the character in it and output the cyphertext.
     * And then continue to read the left of the input till the end of
     * the input.
     */
    while(read_size >= BUFFER_SIZE) {
      cypher(block, read_size);
      
      if(write(STDOUT_FILENO, block, read_size) < 0)
            exit(1);
      
      if((read_size = read(STDIN_FILENO, block, BUFFER_SIZE)) < 0)
	exit(1);
    }
    
    if(read_size == 0)
      continue;
    
    cypher(block, read_size);
    
    if(write(STDOUT_FILENO, block, read_size) < 0)
        exit(1);
  }
  
  return 0;
}
