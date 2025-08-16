#include <stdio.h>
#include <stdint.h>
int
main()
{
	unsigned int j = UINT32_MAX - 100, k = j >> 1;
	int i = j >> 1;
	printf("j: %d (%u) i: %d (%u) k: %d (%u)\n", j, j, i, i, k, k);
}
