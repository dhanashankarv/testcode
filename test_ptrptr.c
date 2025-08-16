#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <errno.h>
#include <string.h>

struct test {
	int a;
	int b;
};

int
test_func(struct test **tp, int *nt)
{
	int i;
	tp = malloc(sizeof (struct test *) * (*nt));
	if (!(tp)) {
		printf("malloc %lu bytes failed: %s\n",
			sizeof(struct test *) * (*nt), strerror(errno));
		return -1;
	}
	printf("Allocated %p for array\n", tp);
	for (i = 0; i < *nt; i++) {
		tp[i] = malloc(sizeof(struct test));
		if (!tp[i]) {
			printf("malloc %lu bytes failed: %s\n",
				sizeof(struct test), strerror(errno));
			return -1;
		}
		printf("Allocated %d: %p\n", i, tp[i]);
	}
	return 0;
}

int
test_func2(uint32_t *nval, uint32_t **valp)
{
	uint32_t *t = *valp;
	int cur = *nval, inc = 5;
	printf("t %p, sizeof(t[0]) %lu\n", t, sizeof(t[0]));
	t = realloc(t, (cur + inc) * sizeof(t[0]));
	if (!t) {
		printf("realloc %lu bytes failed: %s\n",
			inc * sizeof(t[0]), strerror(errno));
		return -1;
	}
	*nval += inc;
	*valp = t;
	return cur;
}

int
main(void)
{
	int i, ret, nt, ndx;
	struct test **t = NULL;
	uint32_t *tval, ntval;

	/*
	nt = 5;
	ret = test_func(t, &nt);
	if (ret < 0) {
		return ret;
	}
	printf("t: %p\n", t);
	for (i = 0; i < nt; i++) {
		printf("t[%d]: %p\n", i, &t[i]);
	}
	*/
	tval = NULL;
	ntval = 0;
	ndx = test_func2(&ntval, &tval);
	printf("tval %p, ntval %u, ndx %u\n", tval, ntval, ndx);
	for (i = ndx; i < (int)ntval; i++) {
		tval[i] = i;
	}
	ndx = test_func2(&ntval, &tval);
	printf("tval %p, ntval %u, ndx %u\n", tval, ntval, ndx);
	for (i = ndx; i < (int)ntval; i++) {
		tval[i] = i;
	}
	ndx = test_func2(&ntval, &tval);
	printf("tval %p, ntval %u, ndx %u\n", tval, ntval, ndx);
	for (i = ndx; i < (int)ntval; i++) {
		tval[i] = i;
	}
	for (i = 0; i < (int)ntval; i++) {
		printf("%u ", tval[i]);
	}
	printf("\n");

	return 0;
}
