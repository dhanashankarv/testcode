#include <stdio.h>

// #define ALIGNED_SIZE(n,s)	(((n)%(s)) ? ((n) - ((n)%(s)) + (s)) : (n))
#define ALIGNED_SIZE(n,s) (((n)&((s)-1)) ? (((n)+(s)-1) & ~((s)-1)) : (n))


int
main(int argc, char *argv[])
{
	int n = 0, s = 4096;
	printf("n %u s %u aligned %d\n", n, s, ALIGNED_SIZE(n, s));
	n = 4096;
	printf("n %u s %u aligned %d\n", n, s, ALIGNED_SIZE(n, s));
	n = 4000;
	printf("n %u s %u aligned %d\n", n, s, ALIGNED_SIZE(n, s));
	n = 4096 * 100 + 1;
	printf("n %u s %u aligned %d\n", n, s, ALIGNED_SIZE(n, s));
}

