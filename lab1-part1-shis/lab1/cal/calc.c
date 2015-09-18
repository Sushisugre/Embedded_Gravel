#include <stdio.h>
#include "math.h"

int main(int argc, char *argv[]){
    printf("test\n");

    if( argc != 3 )
    {
      printf("Invalid operation, please enter 'number operator number'%s\n",);
      return 1;
    }

    int x = atoi(argv[1]);
    int y = atoi(argv[3]);
    char operator = argv[2];
    int result;

    switch (operator){
        case '+':
            result = add(x,y);
            break;
        case '-'
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
            printf("Operation not valid, Supports: +, -, *, /, %% \n");
            return 1;
    }

    printf("%d\n", result);

    return 0;
}