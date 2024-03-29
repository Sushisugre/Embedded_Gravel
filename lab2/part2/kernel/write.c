/*
 * write.c: write syscall
 *
 * Author: Shi Su <shis@andrew.cmu.edu>
 *
 * Date:   Mon Oct 26 14:06:43 EDT 2015
 */

 #include <bits/fileno.h>
 #include <bits/errno.h>
 #include <sys/types.h>
 #include <exports.h>

 #define SDRAM_UPPER 0xa3ffffff
 #define SDRAM_LOWER 0xa0000000
 #define ROM_UPPER 0x00ffffff
 #define ROM_LOWER 0x00000000
 #define MASK 0xff000000


ssize_t write(int fd, const void *buf, size_t count){

    //return error if fd doesn't match stdout
    if(fd!=STDOUT_FILENO){
        // return negative error code 
        return -EBADF;
    }

    unsigned addr_buf = (unsigned)buf;
    char *c_buf = (char*)buf;

    // buffer range exist out side of readable memory
    // StrataFlash ROM of SDRAM, i.e. buf not start with 00 or a3
    if((addr_buf < SDRAM_LOWER
        ||addr_buf > SDRAM_UPPER
        ||(addr_buf + count) >SDRAM_UPPER)
        && ((addr_buf & MASK)!= ROM_LOWER
            ||((addr_buf + count) & MASK)!= ROM_LOWER)){

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
