#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#define ERR_INVALID 1
#define ERR_FAILED  2
#define LOGERR printf
#define zmalloc(x)	calloc(1, x)

struct stormgr {
	char *rcm_token;
};
int
stor_read_token(struct stormgr *stor, char *path)
{
	char hdr[] = "Authorization: ", *buf;
	int fd, ret, token_size, hdr_size, alloc_size;

	hdr_size = strlen(hdr);
	fd = open(path, O_RDONLY);
	if (fd < 0) {
		LOGERR("Failed to open token file %s: %s", path,
			strerror(errno));
		return ERR_INVALID;
	}

	ret = lseek(fd, 0, SEEK_END);
	if (ret < 0) {
		LOGERR("Seek to end of token file %s failed: %s", path,
			strerror(errno));
		return ERR_FAILED;
	}
	if (ret == 0) {
		LOGERR("Token file %s has size %d", path, ret);
		return ERR_INVALID;
	}
	token_size = ret;

	ret = lseek(fd, 0, SEEK_SET);
	if (ret < 0) {
		LOGERR("Seek to start of token file %s failed: %s", path,
			strerror(errno));
		return ERR_FAILED;
	}

	alloc_size = token_size + hdr_size + 10;
	stor->rcm_token = zmalloc(alloc_size);
	if (!stor->rcm_token) {
		LOGERR("Failed to allocate %d sized buffer to read token flow: "
			"%d", alloc_size, ret);
		return ret;
	}

	memcpy(stor->rcm_token, hdr, hdr_size);

	ret = read(fd, &stor->rcm_token[hdr_size], token_size);
	if (ret < 0) {
		LOGERR("Read token file %s failed: %s", path, strerror(errno));
		return ERR_FAILED;
	}

	return 0;
}

int
main(int argc, char *argv[])
{
	struct stormgr stor;
	char tokenpath[] = "/tmp/dhana/token";
	stor_read_token(&stor, tokenpath);
	puts(stor.rcm_token);
}

