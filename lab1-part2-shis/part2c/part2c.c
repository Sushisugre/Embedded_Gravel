#include <stdlib.h>
#include <stdio.h>

/* Updated by Shi Su, AndrewId: shis
 * Oct.02, 2015 */

/**
 * Function: oddball
 * Description: Baseline implementation for finding the number that
 *   occurs only once in the array.
 * Arguments:
 *   arr - pointer to start of integer array
 *   len - number of elements in array
 * Return:
 *   The number that occurs only once
 */
#ifdef DEFAULT
int oddball(int *arr, int len) {
	int i, j;
	int foundInner;
	int result = 0;

	for (i = 0; i < len; i++) {
		foundInner = 0;
		for (j = 0; j < len; j++) {
			if (i == j) {
				continue;
			}
			if (arr[i] == arr[j]) {
				foundInner = 1;
			}
		}
		if (foundInner != 1) {
			result = arr[i];
		}
	}

	return result;
}
#endif

#ifdef OPTIMIZE1

/* 1. Used xor operation to replace inner loop
	because a^a=0, 0^b=b, and xor operation is communtative and associative,
	so we can tell that the result of all array elements xor together will be the single item
   2. Used count-down loop to replace count-up loop  */

int oddball(int *arr, int len) {
	int i;
	int result = 0;

	for(i = len - 1; i >= 0; i--){
		result = result^arr[i];
	}

	return result;
}
#endif

#ifdef OPTIMIZE2
/* 1. Used xor operation to replace inner loop
	because a^a=0, 0^b=b, and xor operation is communtative and associative,
	so we can tell that the result of all array elements xor together will be the single item
   2. Used count-down loop to replace count-up loop  
   3. Unrolling the loop */
int oddball(int *arr, int len) {
	unsigned int i;
	unsigned int result = 0;

	for(i = len - 1; i >= 5; i -= 5){
		result = result^arr[i];
		result = result^arr[i - 1];
		result = result^arr[i - 2];
		result = result^arr[i - 3];
		result = result^arr[i - 4];
		// result = result^arr[i - 5];
	}

	/* handle the leftover from last loop */
	 // for(j = i; j >=0 ; j--){
	 // 	result = result^arr[j];
	 // }

	// if(i >= 5)
	// 	result = result^arr[5];
	if(i >= 4)
		result = result^arr[4];
	if(i >= 3)
		result = result^arr[3];
	if(i >= 2)
		result = result^arr[2];
	if(i >= 1)
		result = result^arr[1];
	result = result^arr[0];

	return result;
}
#endif

/**
 * Function: randGenerator
 * Description: Generate a random array that is in compliance with
 *   lab specification
 * Arguments:
 *   arr - pointer to start of integer array
 *   len - number of elements in array
 * Return:
 *   Unused, but feel free to utilize it.
 */

int randGenerator(int *arr, int len) {
	int i, j, r, rcount;
	for (i = 0; i < len; i++) {
		do {
			rcount = 0;
			r = rand()%(len/2 + 1) + 1;
			for (j = 0; j < i && rcount < 2; j++) {
				if (arr[j] == r) {
					rcount++;
				}
			}
		} while(rcount >= 2);
		arr[i] = r;
		printf("%d ", r);
	}
	printf("\nDone generating\n");
	return 0;
}




