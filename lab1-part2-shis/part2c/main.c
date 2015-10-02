#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int oddball(int *arr, int len);
int randGenerator(int *arr, int len);

#ifndef N
	#define N (40)
#endif

#define ARRLEN (2*(N) - 1)

int main()
{
	clock_t begin, end;
	/* Keep this line here */
	srand(time(NULL));

	int *arr = malloc(ARRLEN*sizeof(int));
	int num;

	if (!arr) {
		return -1;
	}

	randGenerator(arr, ARRLEN);
	begin = clock();
	num = oddball(arr, ARRLEN);
	end = clock();

	printf("Oddball is %d\n", num);
	/* Print out the clock cycle used by the method */
	printf("Clock: %lu\n", (end - begin) );

	return 0;
}

