#include <stdio.h>
typedef enum {
	FOO = 'A',
	BAR = 'B',
	GOO = 'C'
} tenum_t;

#define STR_TENUM(t) ((t) == FOO) ? "FOO" : ((t) == BAR) ? "BAR" : "GOO"
int
main(void)
{
	tenum_t t = FOO;
	printf("sizeof(enum) %lu, %s: %d(%c)\n", sizeof(t), STR_TENUM(t), t, t);
	return 0;
}
