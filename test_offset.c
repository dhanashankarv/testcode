#include <stdio.h>
#define zlist_entry(ptr, type, member)		\
		(type *)((char *)ptr - ((size_t) &((type *)0)->member))

struct test1 {
	int test1a;
	int test1b;
	int test1c;
	int test1d;
	struct test2 {
		int test2a;
		int test2b;
	} test2[3];
	int test1e;
};

int
main()
{
	int index = 1;
	struct test1 t1, *t1p;
	struct test2 *retptr, *t2p = &t1.test2[index];
	int *test2ap = &t1.test2[index].test2a;

	printf("t1 %p, t1.test1d %p, t1.test2[%d] %p, t1.test1e %p\n",
		&t1, &t1.test1d, index, &t1.test2[index], &t1.test1e);
	retptr = zlist_entry(test2ap, struct test2, test2a);
	if (retptr != t2p) {
		printf ("retptr %p, t2p %p\n", retptr, t2p);
		return -1;
	}
	t1p = zlist_entry(retptr, struct test1, test2[index]);
	if (t1p != &t1) {
		printf ("t1p %p, t1 %p\n", t1p, &t1);
		return -1;
	}
	printf ("retptr %p, t1p %p\n", retptr, t1p);

	return 0;
}

