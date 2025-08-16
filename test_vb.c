#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define VB_SECTOR_SIZE 512
typedef unsigned long uint64_t;
typedef unsigned int uint32_t;

static char vb_control_path[] = "/proc/vblock/control";

int
vb_add_device(uint64_t volume_id, uint32_t block_size, uint64_t size_in_blocks,
	      char **vbname)
{
	FILE *fp;
	int ret;
	uint64_t size_in_sectors = (size_in_blocks * block_size)/VB_SECTOR_SIZE;

	fp = fopen(vb_control_path, "w");
	if (!fp) {
		ret = errno;
		goto out;
	}

	ret = fprintf(fp, "vblock add %lu %lu", volume_id, size_in_sectors);
	if (ret < 0) {
		ret = -ret;
		goto out;
	}

	ret = vb_find_device(volume_id, vbname);
out:
	return ret;
}

int
vb_find_device(uint64_t volume_id, char **vbname)
{
	FILE *fp;
	int ret;
	uint64_t vid;
	char vb_name[10];

	fp = fopen(vb_control_path, "r");
	if (!fp) {
		ret = errno;
		goto out;
	}

	while (!feof(fp)) {
		ret = fscanf(fp, "%s vstoreuid=%lu", vb_name, &vid);
		if (ret == EOF) {
			ret = errno;
			goto out;
		}
		if (vid == volume_id) {
			*vbname = strdup(vb_name);
			ret = 0;
			break;
		}
	}
out:
	fclose(fp);
	return ret;
}

int
main(int argc, char *argv[])
{
	int ret;
	char *vb_name;
	ret = vb_add_device(2, 4096, 1048576, &vb_name);
	if (ret) {
		printf("vb_add_device failed: %s\n", strerror(ret));
		return ret;
	}
	printf("Created vblock device %s\n", vb_name);
	free(vb_name);
	return 0;
}
