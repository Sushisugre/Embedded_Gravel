#include <stdio.h>
#include "math.h"

/* 
 * Implementations of the calculator methods
 *
 * Author: Shi Su
 *
 */
 
/* Ignore the overflows... */

 int add(int x, int y){
    return x + y;
 }

 int sub(int x, int y){
    return x - y;
 }

 int mul(int x, int y){
    return x * y;
 }
 
 int cdiv(int x, int y){
    return x / y;
 }

 int mod(int x, int y){
    return x % y;
 }