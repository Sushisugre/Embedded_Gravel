/*
 * rot13.c: ROT13 cipher test application
 *
 * Authors: Shi Su <shis@andrew.cmu.edu>
 *          
 * Date:    Tue Oct 13 11:39:59 EDT 2015
 */

 #include <unistd.h>
 //#include <stdlib.h>
 #include <errno.h>

 typedef int bool;
 #define true 1
 #define false 0
 #define CYCLE_SIZE 26
 #define BUFFER_SIZE 10

 extern ssize_t write(int fd, const void *buf, size_t count);
 extern ssize_t read(int fd, void *buf, size_t count);
 extern void exit(int status);

 void rot(unsigned char* letter, int offset);
 int strlen(const char *str);
 int printf(const char *str);

 int main(int argc, char *argv[]) {
    //

    char *begin = "Start printing command line arguments:\n";
    char *end = "End printing command line arguments.\n";
    char *instruct = "Please enter your message:\n";
    char *div = "\n";
  
    // // print out argv
    printf(begin);
    int i;
    for(i = 0; i<argc; i++){
        // printf(argv[i]);
        printf(div);
    }
    printf(end);
    printf(instruct);

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
        // check if the input ends in this block
            if(num < BUFFER_SIZE
                || (num == BUFFER_SIZE && buffer[BUFFER_SIZE - 1]== '\0')){
                is_finished = true;
            }

        // interate through the characters
            rot(buffer, 13);

        // print out rotated array
        // do not use print method for this may only contain part of str (no \0)
            num = write(STDOUT_FILENO ,buffer, num);
        // syscall error
            if(num == -1){
                exit(1);
            }
        }
    }
    return 0;
}

/* Rotate the characters in a string by specified offset,
    if not in alphabet, let it be */
void rot(unsigned char* p, int offset){

    int i = 0;
    // not at the end of the string or block
    while (*p != '\0' && i < BUFFER_SIZE) {
       unsigned char x = *p;

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

        *p = x;
        p++;
        i++;
    }
}

/*  count the lenght of string,
    add this here since we are not able to use standard lib */
int strlen(const char *str){
    int count = 0;
    while(*str != '\0'){
        count++;
        str++;
    }
    return count;
}

/*  print the content of buffer to stdout using write syscall,
    add this here since we are not able to use standard lib 
    return the return value of write syscall */
int printf(const char *str){
    return write(STDOUT_FILENO, str, strlen(str)+1);
}

