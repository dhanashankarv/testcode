#include <stdio.h>
#include <stdlib.h>

typedef unsigned long uint64_t;

int
loffset_compare(const void *a, const void *b)
{
	uint64_t first = *(uint64_t *)a;
	uint64_t second = *(uint64_t *)b;
	if (first < second) {
		return -1;
	}
	if (first > second) {
		return  1;
	}
	return 0;
}

int
main()
{
	int i;
	uint64_t array[] = {100};
	qsort(array, sizeof(array)/sizeof(array[0]), sizeof(array[0]), loffset_compare);
	for (i = 0; i < sizeof(array)/sizeof(array[0]); i++) {
		printf("%lu ", array[i]);
	}
	printf("\n");
}
