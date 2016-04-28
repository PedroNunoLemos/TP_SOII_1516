#include<stdio.h>
#include "uteis.h"

int aleatorio(int min_num, int max_num, int seed)
{
	int result = 0, low_num = 0, hi_num = 0;
	if (min_num < max_num)
	{
		low_num = min_num;
		hi_num = max_num + 1; // this is done to include max_num in output.
	}
	else {
		low_num = max_num + 1;// this is done to include max_num in output.
		hi_num = min_num;
	}
	srand(time(NULL) * seed);
	result = (rand() % (hi_num - low_num)) + low_num;
	return result;
}