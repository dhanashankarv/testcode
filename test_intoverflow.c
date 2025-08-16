#include <stdio.h>

int
main()
{
	int v1, v2, p2, bsize, dec, diff;
	unsigned long ssize;
	unsigned int a = 1003, b = 1001;

	v1 = 9500 * 33 * 2;
	v2 = 4 * 1024;

	ssize = (unsigned long)v1 * v2;
	printf("%d * %d = %ld\n", v1, v2, ssize);

	bsize = 1024 * 1024;

	while (ssize > 0) {
		dec = ssize > bsize ? bsize : ssize;
		printf("ssize %ld  bsize %d: dec %d\n", ssize, bsize, dec);
		ssize -= dec;
	}

	diff = a - b;
	printf("a %u b %u: a-b: %d\n", a, b, diff);
	diff = b - a;
	printf("b %u a %u: b-a: %d\n", b, a, diff);
}
