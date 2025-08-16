#include <stdio.h>
#include <string.h>
#include <stdint.h>
#ifndef IS_ALIGNED
#define IS_ALIGNED(n,s)		((n) % (s) == 0)
#endif


static inline int
__iobatch_add_iocb(int sliceidx, uint64_t off, int size)
{
	int nlbas;
	uint32_t blocksize = 4096;

	nlbas = size / blocksize;
	printf("sliceidx %d off %lu, size %d rem %d nlbas %d %x\n", sliceidx,
		off, size, size, nlbas, nlbas);
	return 0;
}

int
main()
{
	uint32_t bsize = 4096;
	uint64_t blocksize, offset, off1, off2, sec_offset, end;
	uint64_t slicesize = (1024 * 1024 * 1024);
	int sliceidx1, sliceidx2, size1, size2;
	int size = (1044 * 1024);
	char msg[256];

	offset = 6442131456;
	blocksize = bsize;
	end = offset + size;

	if (IS_ALIGNED(offset, blocksize)) {
		strcpy(msg, "aligned at");
	} else {
		strcpy(msg, "not aligned at");
	}
	printf("%lu %lx %s %lu %lx \n", offset, offset, msg, blocksize, blocksize);
	sliceidx1 = offset / slicesize;
	sliceidx2 = sliceidx1 + 1;
	printf("%d %d\n", sliceidx1, sliceidx2);
	sec_offset = sliceidx2 * slicesize;
	off1 = offset;
	off2 = 0;
	size2 = 0;
	if (end <= sec_offset) {
		sliceidx2 = sliceidx1;
		size1 = size;
	} else {
		size1 = sec_offset - off1;
		off2 = sec_offset;
		size2 = size - size1;
	}
	printf("off1: %lu %lx size1: %u %x\n", off1, off1, size1, size1);
	printf("off2: %lu %lx size2: %u %x\n", off2, off2, size2, size2);
	__iobatch_add_iocb(sliceidx1, off1, size1);
	__iobatch_add_iocb(sliceidx2, off2, size2);
}

