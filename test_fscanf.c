#include <stdio.h>

int
main()
{
	char name[100];
	unsigned long long id;
	int ret;


	while (!feof(stdin)) {
		ret = fscanf(stdin, "%s vstoreuid=%llu", name, &id);
		printf("ret:%d %s %llu\n", ret, name, id);
	}
}
