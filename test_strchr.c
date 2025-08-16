#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#define ROBIN_TRANS_FAILURE -1
#define ROBIN_TRANS_OK 0


int
robin_sender_id_to_host_id(char *sender_id, int sender_id_len, char *host_id, int max_host_id_len)
{
	int host_id_len;
	char *delim;

	if (!sender_id || !host_id) {
		printf("Invalid sender_id (%p) or host_id (%p)\n", sender_id, host_id);
		errno = EINVAL;
		return ROBIN_TRANS_FAILURE;
	}

	delim =	memchr(sender_id, ':', sender_id_len);
	if (!delim) {
		printf("Invalid sender_id (%s) - missing delimiter\n", sender_id);
		errno = EINVAL;
		return ROBIN_TRANS_FAILURE;
	}

	host_id_len = delim - sender_id + 1;
	if (max_host_id_len < host_id_len) {
		host_id_len = max_host_id_len;
	}
	printf("host_id_len: %d\n", host_id_len);
	strncpy(host_id, sender_id, host_id_len);
	printf("setting host_id[%d]: (cur %d %c) to 0\n", host_id_len, host_id[host_id_len - 1], host_id[host_id_len - 1]);
	host_id[host_id_len - 1] = '\0';
	return ROBIN_TRANS_OK;
}

int
main()
{
	char hostname[128];
	char *sender_id[] = {
		"abcd-12345678-adce-9078-Oh_my_god_there_is_lots_of_junk_that_fill_up_the_space_upto_128_bytes_before_the_delimiter_separating_the_thread_id:5555",
	    "abcd-12345678-adce-9078-there_is_lots_of_junk_that_fill_up_the_space_upto_128_bytes_before_the_delimiter_separating_the_thread_id:5555",
		"abcd-12345678-adce-9078-there_is_lots_of_junk_that_fill_up_the_space_upto_128_bytes_before_the_delimiter_separating_the_threa:"
	};
	char *host_id = NULL;
	int host_id_size, i, ret;

	memset(hostname, 1, sizeof(hostname));
	for (i = 0; i < 3; i++) {
		printf("Size of hostname: %lu sender_id[%d]: %lu\n", sizeof(hostname), i, strlen(sender_id[i]));
		if (sizeof(hostname) > strlen(sender_id[i])) {
			host_id = hostname;
			host_id_size = sizeof(hostname);
		} else {
			host_id = (char *) malloc(strlen(sender_id[i]));
			host_id_size = strlen(sender_id[i]);
			printf("Allocated host_id %p of size %d\n", host_id, host_id_size);
		}

		ret = robin_sender_id_to_host_id(sender_id[i], strlen(sender_id[i]), host_id, host_id_size);
		if (ret < 0) {
			perror("robin_sender_id_to_host_id failed");
		} else {
			printf("sender_id[%d] %s, host_id %s\n", i, sender_id[i], host_id);
		}
		if (host_id != hostname) {
			printf("Freeing host_id %p\n", host_id);
			free(host_id);
		}
		printf("\n");
	}
	return 0;
}


