#include <stdio.h>
#include <stdlib.h>
int
func1(int num)
{
	int *testarr[num];
	printf("testarr %p num %d size %lu\n", testarr, num, sizeof(testarr));
}

int
main(int argc, char *argv[])
{
	int num;
	char *eptr;

	if (argc < 2) {
		fprintf(stderr, "Usage %s [count]\n", argv[0]);
		exit(-1);
	}
	if (argv[1][0] == '\0') {
		fprintf(stderr, "Usage %s [count]\n", argv[0]);
		exit(-2);
	}
	num = strtoul(argv[1], &eptr, 10);
	if (*eptr != '\0') {
		fprintf(stderr, "Usage %s [count]\n", argv[0]);
		exit(-3);
	}

	func1(num);
}
