#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <libaio.h>
#include <errno.h>
#include <time.h>
#include <fcntl.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>

#define LOGERR(__format, ...)	fprintf(stderr, "%s():%d "__format"\n",  __func__, __LINE__, ##__VA_ARGS__)
#define LOGINFO(__format, ...)	fprintf(stdout, "%s():%d "__format"\n",  __func__, __LINE__, ##__VA_ARGS__)
typedef unsigned long uint64_t;
#define cpr_seconds_to_milliseconds(seconds)	((seconds) * 1000)
#define cpr_seconds_to_microseconds(seconds)	((seconds) * 1000000)
#define cpr_seconds_to_nanoseconds(seconds)	((seconds) * 1000000000)
#define cpr_current_time()	__cpr_current_time(1000000000)
#define cpr_current_mtime()	__cpr_current_time(1000000)
#define cpr_current_utime()	__cpr_current_time(1000)
#define cpr_current_ntime()	__cpr_current_time(1)

static inline uint64_t
__cpr_current_time(int units_in_ns)
{
	struct timespec now;
	uint64_t ns;

	clock_gettime(CLOCK_REALTIME, &now);
	ns = cpr_seconds_to_nanoseconds(now.tv_sec) + now.tv_nsec;

	return (ns/units_in_ns);
}

char testdata[] = "/root/testdata/testdata";
void
usage(char *name)
{
	LOGERR("Usage: %s <file path>\n", name);
	exit(-1);
}

struct io_args {
	pthread_t issuer;
	pthread_t reaper;
	io_context_t filectx;
	int filefd;
	int datafd;
	int write_size;
	int issuer_status;
	int reaper_status;
	int nevents;
	int block_size;
	int stop;
	int issued;
	int completed;
	uint64_t write_offset;
};

void *
test_io_reaper(void *args)
{
	int ret, i, resp = 0;
	struct io_args *io = (struct io_args *)args;
	uint64_t start, end;
	struct io_event events[io->nevents];
	struct timespec timeout = { .tv_sec = 0, .tv_nsec = 1000000 };

	io->completed = 0;
	io->reaper_status = 0;
	while(!io->stop) {
		start = cpr_current_utime();
		resp = io_getevents(io->filectx, 1, io->nevents, events, &timeout);
		end = cpr_current_utime();
		if (resp <= 0) {
			continue;
		}
		for (i = 0; i < resp; i++) {
			ret = events[i].res;
			if (ret < 0) {
				ret = -ret;
				LOGERR("Write failed: %s", strerror(ret));
				goto out;
			}
			if (ret != io->block_size) {
				LOGERR("Write underflow: expected %d written %d", io->block_size, ret);
				ret = EIO;
				goto out;
			}
			ret = 0;
		}
		io->completed += resp;
		LOGINFO("%ld: Completed %d requests (total %d) in %ld us", io->reaper, i, io->completed, end - start);
	}
out:
	io->reaper_status = ret;
	return args;
}

void *
test_io_issuer(void *args)
{
	int ret, i, cnt = 0;
	struct io_args *io = (struct io_args *)args;
	struct iocb iocb, *iocbs = &iocb;
	char *data = MAP_FAILED;
	char *src, *cur_src;
	uint64_t mapsize, cur_off, start, end;

	io->issuer_status = 0;
	mapsize = io->write_size + io->block_size * 2;
	data = mmap(NULL, mapsize, PROT_READ, MAP_SHARED|MAP_NORESERVE,
		    io->datafd, 0);
	if (data == MAP_FAILED) {
		ret = errno;
		LOGERR("Failed to mmap datafile: %s", strerror(ret));
		io->issuer_status = ret;
		goto out;
	}

	src = (char *)(((uint64_t)data + io->block_size) & ~(io->block_size - 1));
	cnt = 0;
	io->issued = 0;
	while (cnt < io->write_size / io->block_size) {
		start = cpr_current_utime();
		for (i = 0; i < io->nevents; i++) {
			cur_src = src + cnt * io->block_size;
			cur_off = io->write_offset + cnt * io->block_size;
			io_prep_pwrite(&iocb, io->filefd, cur_src, io->block_size, cur_off);
			ret = io_submit(io->filectx, 1, &iocbs);
			if (ret < 0) {
				ret = -ret;
				LOGERR("io_submit failed: %s", strerror(ret));
				goto out;
			}
			cnt++;
			io->issued++;
		}
		end = cpr_current_utime();
		LOGINFO("%ld: Issued %d requests (total %d) in %ld us", io->issuer, i, io->issued, end - start);
		start = cpr_current_utime();
		ret = fsync(io->filefd);
		if (ret < 0) {
			ret = errno;
			LOGERR("fsync failed: %s", strerror(ret));
			goto out;
		}
		end = cpr_current_utime();
		LOGINFO("%ld: Sync after %d requests (total issued %d completed %d) in %ld us", io->issuer, i, io->issued, io->completed, end - start);
	}

out:
	io->issuer_status = ret;
	if (data != MAP_FAILED) {
		munmap(data, mapsize);
	}
	return io;
}

int
main(int argc, char *argv[])
{
	int ret, fd, td;
	char *filepath;
	struct stat sbuf;
	struct io_args io, *tret;

	if (argc < 2) {
		usage(argv[0]);
	}
	filepath = strdup(argv[1]);
	ret = stat(filepath, &sbuf);
	if (ret) {
		ret = errno;
		LOGERR("stat %s failed: %s", filepath, strerror(ret));
		return ret;
	}
	ret = stat(testdata, &sbuf);
	if (ret) {
		ret = errno;
		LOGERR("stat %s failed: %s", testdata, strerror(ret));
		return ret;
	}

	fd = open(filepath, O_RDWR | O_DIRECT);
	if (fd < 0) {
		ret = errno;
		LOGERR("open %s failed: %s", filepath, strerror(ret));
		return ret;
	}
	td = open(testdata, O_RDONLY);
	if (fd < 0) {
		ret = errno;
		LOGERR("open %s failed: %s", testdata, strerror(ret));
		return ret;
	}
	io.nevents = 2048;
	io.filefd = fd;
	io.datafd = td;
	io.write_size = 1024 * 1024 * 1024;
	io.write_offset = 0;
	io.issuer_status = 0;
	io.reaper_status = 0;
	io.block_size = 64 * 1024;
	io.issued = 0;
	io.completed = 0;
	io.stop = 0;

	ret = io_setup(2048, &io.filectx);
	if (ret) {
		ret = -ret;
		LOGERR("io_setup failed: %s", strerror(ret));
		return ret;
	}
	ret = pthread_create(&io.reaper, NULL, test_io_reaper, (void *)&io);
	if (ret) {
		LOGERR("Failed to create IO thread: %s", strerror(ret));
		return ret;
	}
	ret = pthread_create(&io.issuer, NULL, test_io_issuer, (void *)&io);
	if (ret) {
		LOGERR("Failed to create IO thread: %s", strerror(ret));
		return ret;
	}

	ret = pthread_join(io.issuer, (void **)&tret);
	if (ret) {
		LOGERR("Failed to join IO issuer: %s", strerror(ret));
		return ret;
	}
	LOGINFO("%ld: IO issuer thread completed with status: %d", io.issuer, tret->issuer_status);
	while (io.issued != io.completed);
	io.stop = 1;
	ret = pthread_join(io.reaper, (void **)&tret);
	if (ret) {
		LOGERR("Failed to join IO reaper: %s", strerror(ret));
		return ret;
	}
	LOGINFO("%ld: IO reaper thread completed with status: %d", io.reaper, tret->reaper_status);

	ret = io.issuer_status ? io.issuer_status : io.reaper_status;
	io_destroy(io.filectx);
	return ret;
}
