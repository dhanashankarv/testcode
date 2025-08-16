#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int
main()
{
	int ret, i, j;
	struct hostent *he;
	struct in_addr **addr_list;

	he = gethostbyname("eqx01-flash01.robinsystems.com");
	if (!he) {
		perror("gethostbyname failed");
		return -1;
	}

	printf("Primary Name: %s\n", he->h_name);
	printf("Aliases:\n");
	for (i = 0; he->h_aliases[i]; i++) {
		printf("%d: %s\n", i + 1, he->h_aliases[i]);
	}
	printf("Addr type %d, addr len %d\n", he->h_addrtype, he->h_length);
	printf("Addresses:\n");
	addr_list = (struct in_addr **)he->h_addr_list;
	for(i = 0; addr_list[i] != NULL; i++) {
		printf("%d: %s\n", i + 1, inet_ntoa(*addr_list[i]));
	}
}
