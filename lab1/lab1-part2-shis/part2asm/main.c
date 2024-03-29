#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/* This is the function to focus on for optimization */
extern void strTable(char *s, char *d, int sl, int dl);

int main()
{
   /* Assume that dst[i] is always >= 23 (decimal) */
   char src[] = "theinitialstring\n";
   char dst[] = "PENGUINS";
   //char dst[] = "MONKEYS"; // larger han 1 word, smaller than 2 words
   //char dst[] = "CATS";    // 1 word
   //char dst[] = "ELEPHANTS";  // larger than 2 words

   int i, srcLen, dstLen; 
   
   /* chop not word align part */
   srcLen = ((strlen(src) >> 2) << 2); 
   dstLen = ((strlen(dst) >> 2) << 2);

   printf("PRE: src: %s (%d bytes), dst: %s (%d bytes)\n", 
	  src, srcLen, dst, dstLen); 
   
   for (i=0; i<1000; i++)
     strTable(src, dst, srcLen, dstLen); 
   
   printf("POST: src: %s (%d bytes), dst: %s (%d bytes)\n", 
	  src, srcLen, dst, dstLen);
   
   return 1;
}

