/*
 * hello.c: Hello world! with U-Boot Exports API
 *
 * Author: Mike Kasick <mkasick@andrew.cmu.edu>
 * Date:   Sat, 13 Oct 2007 23:51:13 -0400
 */

#include <exports.h>

//typedef unsigned int size_t;
//typedef int ssize_t;

#define STDIN_FILENO 0
#define STDOUT_FILENO 1
#define STDERR_FILENO 2

#define EBADF 9
#define EFAULT 14

/*ssize_t read(int fd, void *buf, size_t count) {
  int input_num, ch;
  
  // Check if the file descriptor matches stdin
  if(fd != STDIN_FILENO)
    return -EBADF;

  // Check if the memory range specified by the buffer and its maximum size
  // exists outside the range of writable memory (SDRAM)
  if(((int)buf < 0xa0000000)||((int)(buf + count - 1) >= 0xa4000000))
    return -EFAULT;
  
  // Begin reading input
  for(input_num = 0; input_num < count;) {
    ch = getc();
    
    if(ch == 4)
      // When getting EOT character
      return input_num;
    else if((ch == 8)||(ch == 127)) { 
      // When getting backspace or delete
      if(input_num != 0) {
	// If there is more than 1 characters been read, remove the last one
	input_num = input_num - 1;
	puts("\b \b");
      }
    } else if((ch == 10)||(ch == 13)) {
      // When getting a newline or carriage return
      putc('\n');
      *((char *)buf + input_num) = '\n';
      input_num = input_num + 1;
      return input_num;
    } else {
      // Otherwise
      *((char *)buf + input_num) = (char)ch;
      input_num = input_num + 1;
      putc((char)ch);
    }
  }

  return input_num;
}

ssize_t write(int fd, const void *buf, size_t count) {
  int i, output_num;
  
  // Check if the file descriptor matches stdout
  if(fd != STDOUT_FILENO)
    return -EBADF;
  
  // Check if the memory range specified by the buffer and its maximum size
  // exists outside the range of writable memory (SDRAM or StrataFlash ROM)
  if((int)buf < 0x00ffffff) {
    if((int)(buf + count - 1) >= 0x00ffffff)
      return -EFAULT;
  } else if ((int)buf >= 0xa0000000) {
    if((int)(buf + count - 1) >= 0xa4000000)
      return -EFAULT;
  } else
    return -EFAULT;

  // Begin writing
  output_num = 0;
  for(i = 0; i < count; i++) {
    putc(((char *)buf)[i]);
    output_num = output_num + 1;
  }
  
  return output_num;
}*/

int main(void) {
  char *buf;
  //char ch;
  //int i;
  int output_num, i, count;

  puts("Please input a string:\n");

  buf = (char *)malloc(sizeof(char)*10);

  count = 10;
  for(i = 0; i < count; i++) {
    buf[i] = i + 97;
  }

  puts("Here is the input line:\n");
  /*for(i = 0; i < input_num; i++) {
    putc(buf[i]);
    }*/
   // Check if the memory range specified by the buffer and its maximum size
  // exists outside the range of writable memory (SDRAM or StrataFlash ROM)
  if((int)buf < 0x00ffffff) {
    if((int)(buf + count - 1) >= 0x00ffffff)
      return -EFAULT;
  } else if ((int)buf >= 0xa0000000) {
    if((int)(buf + count - 1) >= 0xa4000000)
      return -EFAULT;
  } else
    return -EFAULT;

  // Begin writing
  output_num = 0;
  for(i = 0; i < count; i++) {
    putc(buf[i]);
    output_num = output_num + 1;
  }

  putc('\n');

  return 0;
}


