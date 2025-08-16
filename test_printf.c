#include <stdio.h>
#include <stdlib.h>
struct s {
	unsigned long l1, l2;
	unsigned int i1;
};

#define FMT(s) "%lu %lu %u "
#define VAL(s) s.l1, s.l2, s.i1
#define MINOR_NUM(p)	atoi(&((p)[11]))
int
main(void)
{
	unsigned short h = 5557;
	struct s s1= {1, 2, 3};
	int i = 4;
	char p[] = "/dev/vblock25";
	unsigned char c = 128;

	printf("h: %hd %hu 0x%hx\n", h, h, h);
	printf(FMT(s1)"%d\n", VAL(s1), i);
	i = MINOR_NUM(p);
	printf("%s %d\n", p, i);
	printf("unsigned char c = %hhu\n", c);
	return 0;
}
