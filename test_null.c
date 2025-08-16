#include <stdio.h>
#include <string.h>
#include <errno.h>

void
print_test(char *str)
{
        printf("%p %s\n", str, str);
}

int
main()
{
        print_test("");
        print_test(strerror(ENOMEM));
        return 0;
}
