/*
 * rot13.c: ROT13 cipher test application
 *
 * Authors: Shi Su <shis@andrew.cmu.edu>
 *          
 * Date:    The current time & date
 */

 #include <unistd.h>
 #include <stdlib.h>
 #include <errno.h>

 #define true 1
 #define false 0
 #define CYCLE_SIZE 26
 #define BUFFER_SIZE 100

 void rot(unsigned char* letter, int offset);

 int main(void) {

    while(true){
        // may overflow if use signed char
        unsigned char buffer[BUFFER_SIZE];
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

        // interate through the characters
        unsigned char *p = buffer;
        while (*p != '\0') {
            rot(p, 13);
            p++;
        }

        // print out rotated array
        num = write(STDOUT_FILENO ,buffer, num);
        // syscall error
        if(num == -1){
            exit(1);
        }

    }
    return 42;
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
