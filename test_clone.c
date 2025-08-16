#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define ERR_EMPTY 5
int testarr[] = {
	2,
	ERR_EMPTY,
	ERR_EMPTY,
	ERR_EMPTY,
	ERR_EMPTY,
	ERR_EMPTY,
	ERR_EMPTY,
	ERR_EMPTY,
	2,
	0,
	ERR_EMPTY,
	0,
	0,
	0
};

void
print_arr(int *arr, int nelem, char *msg, int valid)
{
	int i;
	printf("%s valid %d\n", msg, valid);
	for (i = 0; i < nelem; i++) {
		printf("%d ", arr[i]);
	}
	printf("\n");
}

int
main()
{
	int *copy;
	int i, ci;
	int nempty = 0;
	int nlbas = sizeof(testarr) / sizeof(testarr[0]);

	copy = calloc(1, sizeof(testarr));
	memcpy(copy, testarr, sizeof(testarr));
	for (i = 0; i < nlbas; i++) {
		if (testarr[i] == ERR_EMPTY) {
			nempty++;
		}
	}
	for (ci = 0; ci < nlbas; ci++) {
		if (testarr[ci] != ERR_EMPTY) {
			break;
		}
	}
	assert(ci <= nempty);
	assert(ci <= nempty && testarr[ci] != ERR_EMPTY);
	for (i = ci + 1; i < nlbas; i++) {
		assert(ci < i);
		if (testarr[i] == ERR_EMPTY) {
			printf("Moving %d to %d\n", i, ci);
			testarr[ci] = testarr[i];
			ci++;
			assert(ci <= nempty);
		}
	}
	assert(ci == nempty);
	print_arr(copy, nlbas, "Original", nlbas);
	print_arr(testarr, nlbas, "Rearranged", ci);
	return 0;
}
