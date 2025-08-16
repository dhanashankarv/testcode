#include <stdio.h>
#define NUM_BUCKETS 256
typedef unsigned long uint64_t;

static inline int
hash_int64(int nbuckets, uint64_t num)
{
	num = (~num) + (num << 21);
	num = num ^ (num >> 24);
	num = (num + (num << 3)) + (num << 8);
	num = num ^ (num >> 14);
	num = (num + (num << 2)) + (num << 4);
	num = num ^ (num >> 28);
	num = num + (num << 31);
	return num % nbuckets;
}

int hashmap[NUM_BUCKETS];

int
main(int argc, char *argv[])
{
	int s, slicesz = 1 * 1024 * 1024 * 1024;
	uint64_t cache_idx = 0, slice_loff = 0, volid = 1;
	for (s = 0; s < 4000; s++) {
		cache_idx = hash_int64(NUM_BUCKETS, slice_loff + volid);
		hashmap[cache_idx]++;
		slice_loff += slicesz;
		printf(" %3ld ", cache_idx);
		if (((s + 1) % 25) == 0) {
			printf("\n");
		}
	}

	for (s = 0; s < NUM_BUCKETS; s++) {
		printf("Bucket %d entries %d\n", s, hashmap[s]);
	}
}
