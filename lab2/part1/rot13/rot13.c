/*
 * rot13.c: ROT13 cipher test application
 *
 * Authors: Shi Su <shis@andrew.cmu.edu>
 *          
 * Date:    Tue Oct 13 11:39:59 EDT 2015
 */

 #include <unistd.h>
 #include <stdlib.h>
 #include <errno.h>

 typedef int bool;
 #define true 1
 #define false 0
 #define CYCLE_SIZE 26
 #define BUFFER_SIZE 10

 void rot(unsigned char* letter, int offset);

 int main(int argc, char *argv[]) {

    // use gdb to check argc, argv
    // may overflow if use signed char
    unsigned char buffer[BUFFER_SIZE];

    // loop and wait for input
    while(true){
        bool is_finished = false;

        // loop until finish reading all the inputs
        while(is_finished != true){

            int num = read(STDIN_FILENO, buffer, BUFFER_SIZE);

        // terminate when zere byte read
        // or interrupted by a signal
            if(num == 0){
                exit(0);
            }
        // syscall error
            if(num == -1){
                exit(1);
            }

            if(num < BUFFER_SIZE
                || (num == BUFFER_SIZE && buffer[BUFFER_SIZE - 1]== '\0')){
                is_finished = true;
            }

            // interate through the characters
            int i = 0;
            unsigned char *p = buffer;
            while (*p != '\0' && i < BUFFER_SIZE) {
                rot(p, 13);
                p++;
                i++;
            }

        // print out rotated array
            num = write(STDOUT_FILENO ,buffer, num);
        // syscall error
            if(num == -1){
                exit(1);
            }
        }


    }
    return 0;
}

/* Rotate character by offset,
if not in alphabet, let it be */
void rot(unsigned char* letter, int offset){

    unsigned char x = *letter;

    if('A' <= x && x <= 'Z'){
        x = x + offset;
        if(x > 'Z'){
            x = x - CYCLE_SIZE;
        }
    }
    else if('a' <= x && x <= 'z'){
        x = x + offset;
        if(x > 'z'){
           x = x - CYCLE_SIZE;
       }
   }

   *letter = x;
}
