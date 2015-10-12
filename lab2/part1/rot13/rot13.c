/*
 * rot13.c: ROT13 cipher test application
 *
 * Authors: Shi Su <shis@andrew.cmu.edu>
 *          
 * Date:    The current time & date
 */

 #include <unistd.h>
 #include <stdlib.h>
 
 #define true 1
 #define false 0

int main(void) {

    while(true){
    
        unsigned char buffer[10];
        int num = read(STDIN_FILENO, buffer, 10);
        if(num == 0){
            // terminate when zere byte read
            // or interrupted by a signal
            exit(0);
        }
        if(num == -1){
            // syscall error
            exit(1);
        }

        write(STDOUT_FILENO ,buffer,10);
    
    }
	return 42;
}
