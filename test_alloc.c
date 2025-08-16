#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <errno.h>

#define ASSERT assert

struct stor_dev {
	int devid;
};

struct stor_volume {
	struct stor_dev **used_devs;
	int		num_used_devs;
};

static inline void *
__zmem_grow_and_return_last(void **recs, int *reccount, size_t recsize)
{
	char *mem;
	size_t len;

	len = ((*reccount) + 1) * recsize;
	mem = (char*)realloc((*recs), len);
	if (mem) {
		memset(&mem[(*reccount) * recsize], 0, recsize);
		*recs = mem;
		mem = &mem[*reccount * recsize];
		*reccount += 1;
	}
	return mem;
}

#define zmem_grow_and_return_last(r,c,s)  __zmem_grow_and_return_last((void **)r, c,s)

static void *
__volume_find_used_dev_or_free_slot(struct stor_volume *vol,
				    struct stor_dev *dev, int *found)
{
	int i;
	void *ptr = NULL;
	*found = 0;
	if (vol->used_devs) {
		for (i = 0; i < vol->num_used_devs; i++) {
			if (!vol->used_devs[i]) {
				if (!ptr) {
					ptr = (void *)&vol->used_devs[i];
					printf("Found free slot (%d) %p\n", i, ptr);
				}
			} else if (vol->used_devs[i]->devid == dev->devid) {
				ASSERT(vol->used_devs[i] == dev);
				ptr = (void *)&vol->used_devs[i];
				*found = 1;
				return ptr;
			}
		}
	}
	return ptr;
}

int
volume_uses_dev(struct stor_volume *vol, struct stor_dev *dev)
{
	int used;
	__volume_find_used_dev_or_free_slot(vol, dev, &used);
	return used;
}

int
volume_add_used_dev(struct stor_volume *vol, struct stor_dev *dev)
{
	int used;
	struct stor_dev **devp;

	devp = (struct stor_dev **)__volume_find_used_dev_or_free_slot(vol, dev,
								       &used);
	if (used) {
		ASSERT(devp && *devp == dev);
		return 0;
	}
	if (!devp) {
		devp = zmem_grow_and_return_last(&vol->used_devs,
						 &vol->num_used_devs,
						 sizeof(struct stor_dev *));
		if (!devp) {
			return ENOMEM;
		}

		printf("Allocated new used dev array %p with %d slots\n", vol->used_devs,
			vol->num_used_devs);
	}
	ASSERT(devp);
	*devp = dev;
	return 0;
}

void
volume_remove_used_dev(struct stor_volume *vol, struct stor_dev *dev)
{
	int used;
	struct stor_dev **devp;

	devp = (struct stor_dev **)__volume_find_used_dev_or_free_slot(vol, dev,
								       &used);
	if (!used) {
		return;
	}
	ASSERT(devp);
	*devp = NULL;
	printf("NULLing dev array at (%lu) %p\n", devp - vol->used_devs, devp);
	return;
}

int
main(int argc, char *argv[])
{
	int ret;
	struct stor_volume vol = {NULL, 0};
	struct stor_dev d1 = {1}, d2 = {2}, d3 = {3}, d4 = {4}, d5 = {5};

	ret = volume_add_used_dev(&vol, &d1);
	ASSERT(!ret);

	ret = volume_add_used_dev(&vol, &d2);
	ASSERT(!ret);

	ret = volume_add_used_dev(&vol, &d3);
	ASSERT(!ret);

	ret = volume_uses_dev(&vol, &d1);
	ASSERT(ret);

	ret = volume_uses_dev(&vol, &d2);
	ASSERT(ret);

	ret = volume_uses_dev(&vol, &d3);
	ASSERT(ret);

	ret = volume_uses_dev(&vol, &d4);
	ASSERT(!ret);

	volume_remove_used_dev(&vol, &d2);
	volume_remove_used_dev(&vol, &d3);

	ret = volume_uses_dev(&vol, &d2);
	ASSERT(!ret);

	ret = volume_uses_dev(&vol, &d3);
	ASSERT(!ret);

	ret = volume_uses_dev(&vol, &d1);
	ASSERT(ret);

	ret = volume_add_used_dev(&vol, &d4);
	ASSERT(!ret);

	ret = volume_add_used_dev(&vol, &d5);
	ASSERT(!ret);

	ret = volume_add_used_dev(&vol, &d1);
	ASSERT(!ret);

	ret = volume_add_used_dev(&vol, &d2);
	ASSERT(!ret);

	ret = volume_add_used_dev(&vol, &d3);
	ASSERT(!ret);

	ret = volume_uses_dev(&vol, &d1);
	ASSERT(ret);

	ret = volume_uses_dev(&vol, &d2);
	ASSERT(ret);

	ret = volume_uses_dev(&vol, &d3);
	ASSERT(ret);

	ret = volume_uses_dev(&vol, &d4);
	ASSERT(ret);

	ret = volume_uses_dev(&vol, &d5);
	ASSERT(ret);
	return 0;
}
