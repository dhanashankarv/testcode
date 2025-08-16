#include <stdio.h>
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

int
main(void)
{
	printf("%d\n", hash_int64(256, 1806033747976));
	return 0;
}
