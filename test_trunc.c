#include <stdio.h>
#include <stdint.h>

#define TRUNC_SIZE (1024 * 1024 * 1024UL)

int
main(void)
{
	uint64_t l = 2 * TRUNC_SIZE + (234234);
	printf("%lu 0x%lX 0x%lX\n", TRUNC_SIZE, TRUNC_SIZE, ~(TRUNC_SIZE - 1));
	printf("%lu: %lu %lu\n", l, (l / TRUNC_SIZE) * TRUNC_SIZE, l & ~(TRUNC_SIZE - 1));
	return 0;
}
