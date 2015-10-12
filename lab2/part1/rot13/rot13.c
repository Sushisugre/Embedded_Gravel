/*
 * rot13.c: ROT13 cipher test application
 *
 * Authors: Shi Su <shis@andrew.cmu.edu>
 *          
 * Date:    The current time & date
 */

 #include <unistd.h>

int main(void) {
    unsigned char buffer[10];
    int num = read(STDIN_FILENO, buffer, 10);
    write(STDOUT_FILENO ,buffer,10);

	return 42;
}
