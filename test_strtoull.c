#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

char num[] = "-1";
int
main(int argc, char *argv[])
{
	unsigned long long val;
	char *endp;
	errno = 0;
	val = strtoull(num, &endp, 10);
	printf("str %s val %llu errno %d\n", num, val, errno);
}
