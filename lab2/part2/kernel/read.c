/*
 * read.c:
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
 #define SDRAM_LOWER 0xa0000000
 // Special characters
 #define EOT 4
 #define BACKSPACE 8
 #define DELETE 127
 #define NEWLINE 10
 #define CARRIAGE_RETURN 13


  ssize_t read(int fd, void *buf, size_t count){
    //return error if fd doesn't match stdout
    if(fd!=STDIN_FILENO){
    // return negative error code 
        return -EBADF;
    }

    unsigned addr_buf = (unsigned)buf;
    char *c_buf = (char*)buf;

    // buffer range exist out side of writable memory SDRAM
    // i.e. address not start with a3
    if( addr_buf < SDRAM_LOWER
        ||addr_buf > SDRAM_UPPER
        ||(addr_buf + count) >SDRAM_UPPER){

        return -EFAULT;
    }

    ssize_t r_count = 0;

    while(r_count < count){
        // call uboot function
        char ch = getc();
        // on EOT, return with current read value
        if(ch == EOT) break;
        else if(ch == BACKSPACE || ch == DELETE){
            // count -1 and leave the last character put in the
            // buffer to be overwrite in next round
            r_count--;
            puts("\b \b");
        }
        else if(ch == NEWLINE || ch == CARRIAGE_RETURN){
            c_buf[r_count] = ch;
            putc(NEWLINE);
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

