#include <stdio.h>
#if 0
#include "/vault/work/robin/3.0/src/include/robin/cpr.h"
#include "/vault/work/robin/3.0/src/include/robin/util.h"
#include "/vault/work/robin/3.0/src/servers/rdvm/ondisk.h"

struct rdvm_slice_1 {
	struct hash_link		cache_link;
	cpr_atomic_t			refcnt;

	cpr_mutex_t			lock;
	cpr_cond_t			cond;

	struct rdvm_dev			*dev;
	struct rdvm_volume		*vol;

	struct slice_metadata_ondisk	hdr;

	/* List of snapshots for this slice */
	cpr_rwlock_t			snaplock;
	struct zlist_head		snaplist;
	struct rdvm_snapshot		*cur_snap;
	int				snapcount;

	uint32_t			flags;

	/* array of segments that hold data for all snapshots in this slice */
	cpr_rwlock_t			seglock;
	int				segcount;
	uint32_t			segseqnum;
	struct segment_ptr		*segments;

	uint64_t			gc_reclaim_space;
	cpr_spinlock_t			gc_lock;
	uint8_t				gc_state;
};

struct rdvm_slice_2 {
	struct hash_link		cache_link;
	cpr_atomic_t			refcnt;

	cpr_mutex_t			lock;
	cpr_cond_t			cond;

	uint32_t			flags;

	cpr_spinlock_t			gc_lock;
	uint8_t				gc_state;
	uint64_t			gc_reclaim_space;

	struct rdvm_dev			*dev;
	struct rdvm_volume		*vol;

	struct slice_metadata_ondisk	hdr;

	/* List of snapshots for this slice */
	cpr_rwlock_t			snaplock;
	int				snapcount;
	struct zlist_head		snaplist;
	struct rdvm_snapshot		*cur_snap;


	/* array of segments that hold data for all snapshots in this slice */
	cpr_rwlock_t			seglock;
	int				segcount;
	uint32_t			segseqnum;
	struct segment_ptr		*segments;
};
#endif
typedef unsigned int uint32_t;
typedef unsigned long uint64_t;

struct raw_write_io_lba_attr {
	uint32_t	seqnum;
	uint32_t	poffset;
	uint32_t	psize;
	uint32_t	cksum;
	uint32_t	lpos;
	uint32_t	compress_algo;
	uint32_t	crypto_algo;
	uint32_t	idx_poff;
	uint32_t	ipage_poff;
	uint32_t	padding;

	uint64_t	generation;
	uint64_t	loffset;
	uint64_t	epoch;
} __attribute__((__packed__));

struct rpc_write_io_attrs {
	uint32_t		flags;  // Per IO flags
	uint32_t		seqnum; // IO seqnum
	uint32_t		priority; // Per IO priority
	uint32_t		nlbas;	// LBAs for the IO request
	struct raw_write_io_lba_attr	lbas[0];
} __attribute__((__packed__));

int
main(void)
{
	//printf("size of 1: %lu, size of 2: %lu\n", sizeof(struct rdvm_slice_1), sizeof(struct rdvm_slice_2));
	printf("Size of uint32_t %ld uint64_t %ld\n", sizeof(uint32_t), sizeof(uint64_t));
	printf("size of rpc_write_io_attrs: %lu, size of raw_Write_io_lba_attr: %lu\n", sizeof(struct rpc_write_io_attrs), sizeof(struct raw_write_io_lba_attr));
	return 0;
}
