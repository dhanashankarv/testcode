#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DIM(x) (sizeof(x)/sizeof(*(x)))

static const char	*sizes[] = { "EiB", "PiB", "TiB", "GiB", "MiB", "KiB", "B" };
static const uint64_t	exbibytes = 1024ULL * 1024ULL * 1024ULL * 1024ULL * 1024ULL * 1024ULL;

char *
calculateSize(uint64_t size)
{
	char	 *result = (char *) malloc(sizeof(char) * 20);
	uint64_t  multiplier = exbibytes;
	int i;

	for (i = 0; i < DIM(sizes); i++, multiplier /= 1024) {
		if (size < multiplier) {
		continue;
		}
		if (size % multiplier == 0) {
		sprintf(result, "%" PRIu64 " %s", size / multiplier, sizes[i]);
		} else {
		sprintf(result, "%.1f %s", (float) size / multiplier, sizes[i]);
		}
		return result;
	}
	strcpy(result, "0");
	return result;
}
