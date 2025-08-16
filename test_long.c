#include <stdio.h>
#include <string.h>
#include <errno.h>

#define ERROR printf
#define ROBIN_TRANS_FAILURE -1
#define ROBIN_TRANS_OK 0

struct robin_disk_data {
	char *wwn;
	unsigned long length;
	unsigned long phy_sector_size;
	unsigned long log_sector_size;
	unsigned short port;
};

struct robin_local_disk_data {
	struct robin_disk_data dd;
	char localpath[16]; // /dev/sd[a-z][a-z][0-9]
};

static int
populate_sblock_device_details(const char *devpath, struct robin_local_disk_data *ldd)
{
        FILE *f;
        char dev_name[16];
        char path[128];
        int ret;
        unsigned long nr_sectors;

        ret = sscanf(devpath, "/dev/%s", dev_name);
        if (ret <= 0) {
                ERROR("Device path %s is not valid\n", devpath);
                return ROBIN_TRANS_FAILURE;
        }

        // Get the number of sectors for sblock device
        snprintf(path, 128, "/sys/block/%s/size", dev_name);
        f = fopen(path, "r");
        if (f == NULL) {
                ERROR("Failed to open %s to get sblock device %s details (errno=%d(%s))\n", path, devpath, errno, strerror(errno));
                return ROBIN_TRANS_FAILURE;
        }

        ret = fscanf(f, "%lu", &nr_sectors);
        if (ret <= 0) {
                ERROR("Failed to get the number of sectors for block device %s\n", devpath);
                return ROBIN_TRANS_FAILURE;
        }
        fclose(f);

        // Get the physical sector size for sblock device
        snprintf(path, 128, "/sys/block/%s/queue/physical_block_size", dev_name);
        f = fopen(path, "r");
        if (f == NULL) {
                ERROR("Failed to open %s to get sblock device %s details (errno=%d(%s))\n", path, devpath, errno, strerror(errno));
                return ROBIN_TRANS_FAILURE;
        }

        ret = fscanf(f, "%lu", &ldd->dd.phy_sector_size);
        if (ret <= 0) {
                ERROR("Failed to get physical sector size for block device %s\n", devpath);
                return ROBIN_TRANS_FAILURE;
        }
        fclose(f);

        // Get the logical sector size for sblock device
        snprintf(path, 128, "/sys/block/%s/queue/logical_block_size", dev_name);
        f = fopen(path, "r");
        if (f == NULL) {
                ERROR("Failed to open %s to get sblock device %s details (errno=%d(%s))\n", path, devpath, errno, strerror(errno));
                return ROBIN_TRANS_FAILURE;
        }

        ret = fscanf(f, "%lu", &ldd->dd.log_sector_size);
        if (ret <= 0) {
                ERROR("Failed to get logical sector size for block device %s\n", devpath);
                return ROBIN_TRANS_FAILURE;
        }
        fclose(f);

        ldd->dd.length = nr_sectors * ldd->dd.log_sector_size;
        ldd->dd.port = 0;
        strncpy(ldd->localpath, devpath, 16);

        return ROBIN_TRANS_OK;
}

int
main()
{
    struct robin_local_disk_data ldd;
    int ret;
    ret = populate_sblock_device_details("/dev/sdb", &ldd);
    if (ret < 0) {
            printf("populate_sblock_device_details(sdb) failed\n");
            return ret;
    }
    printf("wwn: %s, length: %lu (0x%lx), phy: %ld, log: %ld\n", ldd.dd.wwn, ldd.dd.length, ldd.dd.length, ldd.dd.phy_sector_size, ldd.dd.log_sector_size);
}
