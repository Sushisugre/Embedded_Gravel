/*
 * write.c:
 *
 * Author: Shi Su <shis@andrew.cmu.edu>
 *
 * Date:   The current time & date
 */

 #include <bits/fileno.h>
 #include <bits/errno.h>
 #include <sys/types.h>
 #include <exports.h>

 #define SDRAM_UPPER 0xa3ffffff
 #define SDRAM_LOWER 0xa3000000
 #define MASK 0xff000000


ssize_t write(int fd, const void *buf, size_t count){

    //return error if fd doesn't match stdout
    if(fd!=STDOUT_FILENO){
        // return negative error code 
        return -EBADF;
    }

    unsigned addr_buf = (unsigned)buf;
    char *c_buf = (char*)buf;

    // buffer range exist out side of writable memory SDRAM
    // i.e. address not start with a3
    if((addr_buf & MASK)!= SDRAM_LOWER
        ||((addr_buf + count) & MASK)!= SDRAM_LOWER){

        return -EFAULT;
    }

    ssize_t w_count = 0;
    // write to stdout until reach limited count
    while(w_count < count){

        char ch = (char)c_buf[w_count];
        // call uboot API to display the character
        putc(ch);
        w_count ++;
    }

    return w_count;
}
