#include <sys/uio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <linux/fcntl.h>

#define PAGE_SIZE (4 * 1024)
int
main(int argc, char *argv[])
{
	int fd, off, i, ret, bufsize = 2 * 1024 * 1024;
	size_t wsize, fsize;
	struct iovec iovec[2];
	char *buf;

	//fd = open(argv[1], O_RDWR);
	fd = open(argv[1], O_DIRECT|O_SYNC|O_RDWR);
	if (fd < 0) {
		fprintf(stderr, "open %s failed: %s\n", argv[1], strerror(errno));
		return fd;
	}

	fsize = llseek(fd, 0, SEEK_END);
	if (fsize < 0) {
		fprintf(stderr, "lseek %s end of file failed: %s\n", argv[1],
				strerror(errno));
		return fd;
	}

	wsize = atol(argv[2]);
	if (wsize > fsize) {
		fprintf(stderr, "%s is of size %lu. Cannot write to %lu\n", argv[1],
				fsize, wsize);
		return -1;
	}

	ret = posix_memalign((void **)&buf, PAGE_SIZE, bufsize);
	if (ret < 0) {
		fprintf(stderr, "posix_memalign %d (aligned at %d) failed: %s\n",
				bufsize, PAGE_SIZE, strerror(errno));
		return -ret;
	}

	for (i = 0; i < bufsize; i++) {
		buf[i] = 'a';
	}

	iovec[0].iov_base = buf;
	iovec[0].iov_len = bufsize;
	iovec[1].iov_base = NULL;
	iovec[1].iov_len = 0;

	fprintf(stdout, "Attempt to write to %s till %lu\n", argv[1], wsize);

	for (off = 0; off < wsize; off += bufsize) {
		ret = pwritev(fd, iovec, 1, off);
		if (ret < 0) {
			fprintf(stderr, "pwritev %s at %d size %u failed: %s\n", argv[1],
					off, bufsize, strerror(errno));
		} else if (ret != iovec[0].iov_len) {
			fprintf(stderr, "Partial write offset %u: req %lu, written %u\n",
					off, iovec[0].iov_len, ret);
		}
	}

	ret = close(fd);



	return ret;
}
