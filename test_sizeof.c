#include <stdio.h>
#include <stdint.h>
struct block_entry_ondisk {
	uint32_t		rel_blockid;
	uint32_t		enc_valid_poffset;
	uint32_t		enc_compress_encrypt_psize;
	uint32_t		cksum;
};

struct discard_blocks_entry_ondisk {
	uint32_t	start_rel_blkid;
	uint32_t	enc_valid_blkcnt;
	uint32_t	psize; // Should be 0
	uint32_t	cksum; // Should be 0
};

union index_entry_ondisk {
	struct block_entry_ondisk blkentry;
	struct discard_blocks_entry_ondisk discardentry;
	uint32_t segopentry;
};

struct index_page_hdr_ondisk {
	uint32_t		cksum;
	int			seg_sliceidx;
	uint32_t		seg_seqnum;
	uint64_t		slice_genkey;
	uint64_t		seg_genkey;
};

#define ROBIN_REDO_VERSION	1

#define REDO_INVALID_ENTRY	0
#define REDO_INDEX_ENTRY	1
#define REDO_DISCARD_ENTRY	2
#define REDO_SEGOP_ENTRY	3
#define REDO_MAX_ENTRY		4

#define REDO_SEGOP_ASSIGN	1
#define REDO_SEGOP_RELEASE	2

struct redo_page_hdr_ondisk {
	uint32_t		cksum;
	uint16_t		size;
	uint16_t		nentries;
};

struct redo_index_entry_ondisk {
	uint32_t			enc_entry_type_poff;
	union index_entry_ondisk	index_entry;
};
#define REDO_INDEX_ENTRY_TYPE(re) (((re)->enc_entry_type_poff >> 28) & 0xF)
#define REDO_INDEX_ENTRY_POFF(re) ((re)->enc_entry_type_poff & ~(0xF << 28))

struct redo_entry_ondisk {
	uint16_t			version;
	uint16_t			nentries;
	int				segsliceidx;
	int				segmapidx;
	uint32_t			segseqnum;
	uint64_t			txnid;
	struct redo_index_entry_ondisk	indexentries[0];
};

int
main(int argc, char *argv[])
{
	char test[80];
	char *eighty_ptr[80];
	printf("sizeof eighty_ptr %lu\n", sizeof(eighty_ptr));
	printf("sizeof test %lu\n", sizeof(test));
	printf("sizeof(ssize_t) %lu\n", sizeof(ssize_t));
	printf("sizeof(size_t) %lu\n", sizeof(size_t));
	printf("sizeof(int) %lu\n", sizeof(int));
	printf("sizeof(struct redo_entry_ondisk) %lu\n", sizeof(struct redo_entry_ondisk));
	printf("sizeof(struct redo_index_entry_ondisk) %lu\n", sizeof(struct redo_index_entry_ondisk));
}
