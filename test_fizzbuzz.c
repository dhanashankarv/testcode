#include <stdio.h>

int
main()
{
	int print_num = 1, count = 0;
	for (count = 0; count <= 100; count++) {
		if ((count % 3) == 0) {
			printf("Fizz");
			print_num = 0;
		}
		if ((count % 5) == 0) {
			printf("Buzz");
			print_num = 0;
		}
		if (print_num) {
			printf("%d", count);
		}
		printf("\n");
		print_num = 1;
	}
}

