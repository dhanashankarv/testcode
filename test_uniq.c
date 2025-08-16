#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
typedef unsigned int uint32_t;
#define ASSERT assert
int
segidx_cmp(const void *a, const void *b)
{
	uint32_t s1 = *(uint32_t *)a;
	uint32_t s2 = *(uint32_t *)b;
	return s1 - s2;
}
int
uniq(uint32_t *segidx, int nblocks)
{
	int i, c;
	for (i = 0, c = 0; i < nblocks; i++) {
		if (!segidx[i] || (segidx[c] == segidx[i])) {
			continue;
		}
		ASSERT(segidx[i] > segidx[c]);
		if (segidx[c]) {
			c++;
		}
		if (c < i && segidx[c] < segidx[i]) {
			segidx[c] = segidx[i];
		}
	}
	if (segidx[c]) {
		c++;
	}
	return c;
}

void
print_array(uint32_t *segidx, int nidx)
{
	int i;
	for (i = 0; i < nidx; i++) {
		printf("%d ", segidx[i]);
	}
	printf("\n");
}

int
main()
{
	int nuniq, nentries;
	//uint32_t segidx[] = {1, 3, 2, 4, 0, 6, 8, 0, 7, 9, 5, 2, 4, 7, 1, 9, 8, 6, 2};
	uint32_t segidx[] = {5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5};
	nentries = sizeof(segidx)/sizeof(segidx[0]);
	printf("No. of entries: %d\n", nentries);
	print_array(segidx, nentries);

	qsort(segidx, nentries, sizeof(uint32_t), segidx_cmp);
	printf("Sorted: %d entries\n", nentries);
	print_array(segidx, nentries);

	nuniq = uniq(segidx, nentries);
	printf("No. of uniq values: %d\n", nuniq);
	print_array(segidx, nentries);
}
