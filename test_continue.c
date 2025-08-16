#include <stdio.h>

int
main()
{
	int i;
	for (i = 0; i < 5; i++) {
		if (i != 4) {
			continue;
		}
		printf("%d\n", i);
	}
	printf("%d\n", i);
}
