#include <stdio.h>
#define MIN_VAL 100
#define MULT_FACT (1.5)
#define MAX_VAL 60000

int
main()
{
	unsigned int retry = MIN_VAL;
	int i, total_delay = 0;
	for (i = 0;; i++) {
		if (retry >= MAX_VAL) {
			break;
		}
		retry *= MULT_FACT;
		total_delay += retry;
		if ((i % 1) == 0) {
			printf("MultFact %.2f: Went from %d to %d in %d "
				"retries total_delay %d\n", MULT_FACT,
				MIN_VAL, retry, i + 1, total_delay);
		}
	}
	printf("MultFact %.2f: Went from %d to %d in %d retries total_delay "
		"%d\n", MULT_FACT, MIN_VAL, MAX_VAL, i, total_delay);
}
