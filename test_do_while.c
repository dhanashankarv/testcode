#include <stdio.h>
int
main(void)
{
	int i = 0;
	do {
		printf("Entering loop: %d\n", i);
		if (i < 2) {
			i++;
			continue;
		}
		printf("Breaking loop: %d\n", i);
		break;
	} while(1);
}

