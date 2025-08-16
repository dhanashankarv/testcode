#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>

int
main()
{
	int ret, i;
	char *hostname = "test16.robinsystems.com";
	struct addrinfo hints = {(AI_V4MAPPED | AI_ADDRCONFIG),
							  AF_INET, SOCK_STREAM, 0, 0, NULL, NULL, NULL};
	struct addrinfo *res, *cur;
	struct sockaddr_in *sin;
	ret = getaddrinfo(hostname, NULL, &hints, &res);
	if (ret) {
		printf("getaddrinfo(%s) failed: %d\n", hostname, ret);
		return ret;
	}

	for (cur = res; cur != NULL; cur = cur->ai_next) {
		printf("%d %d %d %d %d\n", cur->ai_flags, cur->ai_family, cur->ai_socktype, cur->ai_protocol, cur->ai_addrlen);
		sin = (struct sockaddr_in *)cur->ai_addr;
		printf("%s\n", inet_ntoa(sin->sin_addr));
		if (cur->ai_canonname) {
			printf("%p\n", cur->ai_canonname);
		}
	}
	freeaddrinfo(res);
}

