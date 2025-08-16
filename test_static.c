#include <stdio.h>
int
testval(void)
{
	static int v = 0;
	return ++v;
}

int
func(void)
{
	static int s = -1;
	if (s < 0) {
		s = testval();
	}
	return s;
}

int
main(int argc, char *argv[])
{
	int s;
	s = func();
	printf("%d\n", s);
	s = func();
	printf("%d\n", s);
}
