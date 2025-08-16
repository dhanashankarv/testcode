#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

#define ASSERT assert

struct rio_lba {
	/* Logical offset of this IO in the volume */
	uint64_t	loffset;
	/* Physical size of the data - normally blocksize, lower if compressed*/
	int		psize;
};
struct rio_devio {
	struct rio_lba *lbas;
	uint32_t nlbas;
};

static int
lba_sort_asc(const void *aptr, const void *bptr)
{
	struct rio_lba *a = (struct rio_lba *)aptr;
	struct rio_lba *b = (struct rio_lba *)bptr;

	if (a->loffset > b->loffset) {
		return 1;
	} else if (a->loffset < b->loffset) {
		return -1;
	} else if (a->loffset == b->loffset) {
		return a->psize - b->psize;
	}

	return 0;
}

static void
lba_merge_ranges(struct rio_devio *dio)
{
	int i, empty, last;

	qsort(dio->lbas, dio->nlbas, sizeof (struct rio_lba), lba_sort_asc);
	empty = 0;
	last = dio->nlbas;
	for (i = 1; i < last; i++) {
		ASSERT(dio->lbas[i].loffset >= dio->lbas[i - 1].loffset);
		if (dio->lbas[i].loffset == dio->lbas[i - 1].loffset) {
			dio->lbas[i - 1].psize = dio->lbas[i].psize;
			empty = i;
		} else if (dio->lbas[i].loffset <=
			  (dio->lbas[i - 1].loffset + dio->lbas[i - 1].psize)) {
			if ((dio->lbas[i].loffset + dio->lbas[i].psize) >
			   (dio->lbas[i - 1].loffset + dio->lbas[i - 1].psize)) {
				dio->lbas[i - 1].psize = (dio->lbas[i].loffset -
							  dio->lbas[i - 1].loffset) +
							  dio->lbas[i].psize;
			}
			empty = i;
		}
		if (empty) {
			memmove(&dio->lbas[empty], &dio->lbas[i + 1],
				(dio->nlbas - (i + 1)) * sizeof(dio->lbas[0]));
			empty = 0;
			last--;
		}
	}
	dio->nlbas = last;
}

void
lba_print_ranges(struct rio_devio *dio)
{
	int i;
	for(i = 0; i < (int)dio->nlbas; i++) {
		printf("%d: %lu %u\n", i, dio->lbas[i].loffset, dio->lbas[i].psize);
	}
}

int blksz = 4 * 1024;
int
main(int argc, char *argv[])
{
	struct rio_lba lbas[] = {
		{35 * blksz, 64 * blksz},
		{0, 32 * blksz},
		{300 * blksz, 16 * blksz},
		{100 * blksz, 32 * blksz},
		{40 * blksz, 32 * blksz},
		{32 * blksz, 16 * blksz},
	};
	struct rio_devio dio = {lbas, sizeof(lbas) / sizeof(lbas[0])};
	printf("INPUT\n");
	lba_print_ranges(&dio);
	lba_merge_ranges(&dio);
	printf("OUTPUT\n");
	lba_print_ranges(&dio);
}
