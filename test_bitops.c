#include <stdio.h>
#include <stdlib.h>
 /* 32 locks to protect the entire bmap */
#define RDVM_BMAP_NUM_BE_LOCKS_BITS	5
#define RDVM_BMAP_NUM_BE_LOCKS		(1 << RDVM_BMAP_NUM_BE_LOCKS_BITS)

 /* A lock covers 32 consecutive blocks */
#define RDVM_BMAP_BE_LOCK_SPAN_BITS	5
#define RDVM_BMAP_BE_LOCK_SPAN		(1 << RDVM_BMAP_BE_LOCK_SPAN_BITS)

#define RDVM_BMAP_TOTAL_LOCK_SPAN	(1 << (RDVM_BMAP_NUM_BE_LOCKS_BITS + RDVM_BMAP_BE_LOCK_SPAN_BITS))

#define SLICE_SIZE_BITS			30
#define SLICE_SIZE			(1ULL << SLICE_SIZE_BITS)
#define SLICE_BLOCK_SIZE_BITS		12
#define SLICE_BLOCK_SIZE		(1UL << SLICE_BLOCK_SIZE_BITS)
#define SNAP_RELLOFFSET(abs_loffset)	(abs_loffset & (SLICE_SIZE - 1))
#define SNAP_RELBLOCKID(abs_loffset)	(SNAP_RELLOFFSET(abs_loffset) >> SLICE_BLOCK_SIZE_BITS)

typedef unsigned long long uint64_t;

static inline int
bmap_lock_start_blk(int slot)
{
	return (slot & ~(RDVM_BMAP_BE_LOCK_SPAN - 1));
}

static inline int
bmap_lock_end_blk(int slot)
{
	return (bmap_lock_start_blk(slot) + RDVM_BMAP_BE_LOCK_SPAN);
}

static inline int
bmap_slot_lockid(int slot)
{
	int set = slot & (RDVM_BMAP_TOTAL_LOCK_SPAN - 1);
	int lockid = set >> RDVM_BMAP_BE_LOCK_SPAN_BITS;
	return lockid;
}

int
main(int argc, char *argv[])
{
	int slot, lock_span, lock_id, start_blk, end_blk;
	uint64_t loffset = (4ULL << SLICE_SIZE_BITS) + (1024 * 1024 * 4) + ((RDVM_BMAP_NUM_BE_LOCKS - 1) * RDVM_BMAP_BE_LOCK_SPAN * SLICE_BLOCK_SIZE);

	printf("Num BMAP locks %d, Per lock span %d blocks, total lock span %d blocks, total blocks per bmap: %ld\n",
	       RDVM_BMAP_NUM_BE_LOCKS, RDVM_BMAP_BE_LOCK_SPAN, RDVM_BMAP_TOTAL_LOCK_SPAN, 1UL << (SLICE_SIZE_BITS - SLICE_BLOCK_SIZE_BITS));
	printf("slice size %llu, block size %lu\n", SLICE_SIZE, SLICE_BLOCK_SIZE);
	slot = SNAP_RELBLOCKID(loffset);
	lock_id = bmap_slot_lockid(slot);
	start_blk = bmap_lock_start_blk(slot);
	end_blk = bmap_lock_end_blk(slot);
	printf("loffset %llu (0x%llx) rel_loffset %llu slot %u lock id %u start blk %d end blk %d\n",
	       loffset, loffset, SNAP_RELLOFFSET(loffset), slot, lock_id, start_blk, end_blk);
}
