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
 #include <string.h>

 #define true 1
 #define false 0

int main(void) {

    while(true){
    
        unsigned char buffer[10];
        int num = read(STDIN_FILENO, buffer, 10);
        printf("print errno %s\n", strerror(errno));

        // terminate when zere byte read
        // or interrupted by a signal
        if(num == 0){
            exit(0);
        }
        // syscall error
        if(num == -1){
            exit(1);
        }

        write(STDOUT_FILENO ,buffer,10);
    
    }
	return 42;
}
