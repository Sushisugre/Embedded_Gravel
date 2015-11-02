/*
 * kernel.c: Kernel main (entry) function
 *
 * Author: Mengjin Yan <myan1@andrew.cmu.edu>
 * Date:   Sat Oct 24 13:59:17 EDT 2015  
 */

#include <exports.h>
#include <bits/fileno.h>
#include <bits/errno.h>

// Assembly function to clean up and exit the kernel
extern void exit(int status);

// Assembly function of SWI handler
extern void S_Handler();

// Assembly function to set up user mode and the stack
// Call user function and return
// Transfer back to svc mode
extern int User_Function(int argc, char *argv[]);

// Read system call
ssize_t read(int fd, char *buf, size_t count) {
  int input_num, ch;
  
  // Check if the file descriptor matches stdin
  if(fd != STDIN_FILENO)
    return -EBADF;

  // Check if the memory range specified by the buffer and its maximum size
  // exists outside the range of writable memory (SDRAM)
  if(((size_t)buf < 0xa0000000)||(((size_t)buf + count) >= 0xa3ffffff))
    return -EFAULT;
  
  // Begin reading input
  for(input_num = 0; input_num < count;) {
    ch = getc();
    
    if(ch == 4)
      // When getting EOT character
      //return input_num;
      break;
    
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
      buf[input_num] = '\n';
      input_num = input_num + 1;
      //return input_num;
      break;
    } else {
      // Otherwise
      buf[input_num] = (char)ch;
      input_num = input_num + 1;
      putc((char)ch);
    }
  }

  return input_num;
}

// write system call
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
}

// The swi handler
void C_SWI_handler(unsigned swi_num, unsigned *regs) {
  switch (swi_num) {
  case 0x900001:// when calling exit system call
    exit(regs[0]);
    break;
  case 0x900003:// when calling read system call
    regs[0] = read(regs[0], (void *)regs[1], regs[2]);
    break;
  case 0x900004:// when calling write system call
    regs[0] = write(regs[0], (void *)regs[1], regs[2]);
    break;
  default:
    puts("Exception(0x0badc0de):Invalid system call.\n");
    exit(0x0badc0de);
  }
}

// Function to install software interrupt
void Install_Handler(unsigned *old_handler, unsigned **handler_addr) {
  unsigned offset, a, b;
  unsigned *vector = (unsigned *) 0x08;
  
  a = *vector;
  b = a & 0xFFFFF000;
  
  // Judge if the the instruction "ldr pc, [pc, #imm12]"
  if((b != 0xe51ff000)&&(b != 0xe59ff000)) {
    puts("Error(0x0badc0de): Unrecognized instruction.\n");
    exit(0x0badc0de);
  }

  offset = (*vector) & 0x00000FFF;
  if(b == 0xe51ff000) {
    // The offset needs to be subtract from the address
    *handler_addr = (unsigned *)(*(unsigned *)((unsigned)vector + 0x08 - offset));
  } else {
    // The offset needs to be add to the address
    *handler_addr = (unsigned *)(*(unsigned *)((unsigned)vector + 0x08 + offset));
  }

  // Save the original handler in old_handler
  old_handler[0] = **handler_addr;
  old_handler[1] = *((unsigned *)((unsigned)(*handler_addr)+4));
  
  // Write in the new instruction 
  **handler_addr = 0xe51ff004;
  *((unsigned *)((unsigned)(*handler_addr)+4)) = (unsigned)&S_Handler;
}

// Function to reinstall the old software handler
void Restall_Old_Handler(unsigned *old_handler, unsigned **handler_addr) {
  **handler_addr = old_handler[0];  
  *((*handler_addr)+4) = old_handler[1];
}

int main(int argc, char *argv[]) {
  unsigned old_swi[2];
  unsigned *handler_addr;
  int exit_value;

  handler_addr = 0;
  Install_Handler(old_swi, &handler_addr);
  exit_value = User_Function(argc, argv);
  Restall_Old_Handler(old_swi, &handler_addr);

  return exit_value;
}
