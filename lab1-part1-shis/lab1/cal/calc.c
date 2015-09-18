#include <stdio.h>
#include <stdlib.h>
#include "math.h"

#define OPERATION_SIZE 30
#define TRUE 1
#define FALSE 0

int main(int argc, char *argv[]){

    char input[OPERATION_SIZE];
    int x = 1;
    int y = 2;
    char operator = '+';
    int result;
    int is_valid;

    // keep looping until exit
    while(TRUE){

        is_valid = TRUE;
        // fgets(input, OPERATION_SIZE, stdin);

        // // FIXME: 
        scanf("%d %c %d", &x, &operator, &y);
        // // printf("%s\n", input);


        // printf("x: %d\n",x );
        // printf("operator %c\n", operator );
        // printf("y: %d\n",y );


        switch (operator){
            case '+':
                result = add(x,y);
                break;
            case '-':
                result = sub(x,y);
                break;
            case '*':
                result = mul(x,y);
                break;
            case '/':
                result = cdiv(x,y);
                break;
            case '%':
                result = mod(x,y);
                break;
            default:
                is_valid = FALSE;
                printf("Operator not valid, Supports: +, -, *, /, %% \n");
                break;
        }

        if(is_valid)
            printf("%d\n", result);
    }

    return 0;
}