#include <string.h>
#include <strings.h>
#include <stdio.h>

int
is_exp_of_two(unsigned long long val, int *exp)
{
	int s = ffsll(val);
	unsigned long long rem;
	rem = val >> s;
	*exp = s - 1;
	return !rem;
}

int
main(int argc, char *argv[])
{
	unsigned long long num = 1 * 1024 * 1024 * 1024;
	unsigned long long div = 32 * 1024 * 1024;
	unsigned long long vals[] = {num, div, num * div, 1, 1000000, 1000000000};
	int rshift = ffsll(div) - 1;
	int val = num >> rshift;
	int i, exp, ret;
	printf("ffs(%lld(0x%llx)) = %d\n", div, div, rshift + 1);
	printf("%lld(0x%llx)/%lld(0x%llx) = %d(0x%x)\n", num, num, div, div, val, val);
	for (i = 0; i < sizeof(vals)/sizeof(vals[0]); i++) {
		ret = is_exp_of_two(vals[i], &exp);
		if (ret) {
			printf("ret %d: %llu is 2^%d\n", ret, vals[i], exp);
		} else {
			printf("ret %d: %llu is NOT an exp of 2\n", ret, vals[i]);
		}
	}
	return 0;
}

