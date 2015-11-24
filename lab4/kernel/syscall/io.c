/** @file io.c
 *
 * @brief Kernel I/O syscall implementations
 *
 * @author Shi Su <shis@andrew.cmu.edu>
 *         Mengjin Yan <mengjinyan@cmu.edu>
 *         
 * @date 2015-11-24
 */

#include <types.h>
#include <bits/errno.h>
#include <bits/fileno.h>
#include <arm/physmem.h>
#include <syscall.h>
#include <exports.h>
#include <kernel.h>

 #define MASK 0xff000000

 // Special characters
 #define EOT_CHAR 0x04
 #define DEL_CHAR 0x7f
 #define BACK_CHAR 8
 #define NEWLINE_CHAR 10
 #define CARR_RET_CHAR 13


/* Read count bytes (or less) from fd into the buffer buf. */
ssize_t read_syscall(int fd __attribute__((unused)), void *buf __attribute__((unused)), size_t count __attribute__((unused)))
{

    //return error if fd doesn't match stdout
    if(fd!=STDIN_FILENO){
    // return negative error code 
        return -EBADF;
    }

    unsigned addr_buf = (unsigned)buf;
    char *c_buf = (char*)buf;

    // buffer range exist out side of writable memory SDRAM
    // i.e. address not start with a3
    if( addr_buf < RAM_START_ADDR
        ||addr_buf >= RAM_END_ADDR
        ||(addr_buf + count) >= RAM_END_ADDR){

        return -EFAULT;
    }

    size_t r_count = 0;

    while(r_count < count){
        // call uboot function
        char ch = getc();
        // on EOT, return with current read value
        if(ch == EOT_CHAR) break;
        else if(ch == BACK_CHAR || ch == DEL_CHAR){
            // count -1 and leave the last character put in the
            // buffer to be overwrite in next round
            if(r_count > 0){
                r_count--;
                puts("\b \b");
            }
        }
        else if(ch == NEWLINE_CHAR || ch == CARR_RET_CHAR){
            // put a newline character in the buffer
            c_buf[r_count] = NEWLINE_CHAR;
            putc(NEWLINE_CHAR);
            r_count++;
            break;
        }
        else{
            c_buf[r_count] = ch;
            putc(ch);
            r_count ++;
        }
    }

    return r_count;
	
}

/* Write count bytes to fd from the buffer buf. */
ssize_t write_syscall(int fd  __attribute__((unused)), const void *buf  __attribute__((unused)), size_t count  __attribute__((unused)))
{

  
    //return error if fd doesn't match stdout
    if(fd!=STDOUT_FILENO){
        // return negative error code 
        return -EBADF;
    }

    unsigned addr_buf = (unsigned)buf;
    char *c_buf = (char*)buf;

    // buffer range exist out side of readable memory
    // StrataFlash ROM of SDRAM, i.e. buf not start with 00 or a3
    if((addr_buf < RAM_START_ADDR
        ||addr_buf >= RAM_END_ADDR
        ||(addr_buf + count) >= RAM_END_ADDR)
        && ((addr_buf & MASK)!= FLASH_START_ADDR
            ||((addr_buf + count) & MASK)!= FLASH_START_ADDR)){

        return -EFAULT;
    }

    size_t w_count = 0;
    // write to stdout until reach limited count
    while(w_count < count){

        char ch = (char)c_buf[w_count];
        // call uboot API to display the character
        putc(ch);
        w_count ++;
    }

    return w_count;
	
}

