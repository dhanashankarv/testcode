#include <stdio.h>

#define ERR_MEMORY	1
#define ERR_FAILED	2
#define ERR_EMPTY	3
struct vol_snapshot {
	unsigned int snapid;
	struct vol_snapshot *parent;
	int numchildren;
}

int numsnaps;
struct vol_snapshot *vsnaps;

int
setup(int nsnaps)
{
	int i;
	vsnaps = calloc(nsnaps + 1, sizeof(struct vol_snapshot));
	if (!vsnaps) {
		return ERR_MEMORY;
	}
	for (i = 1; i <= nsnaps; i++) {
		unsigned int paridx;
		vsnaps[i].snapid = i;
		paridx = (i & 1) ? i / 2 : (i - 1)/2;
		vsnaps[i].parent = paridx ? &vsnaps[paridx] : NULL;
		if (2 * i + 1 <= nsnaps) {
			vsnaps[i].numchildren = 2;
		} else if (2 * i <= nsnaps) {
			vsnaps[i].numchildren = 1;
		} else {
			vsnaps[i].numchildren = 0;
		}
	}
	numsnaps = nsnaps;
	return 0;
}

struct vol_snapshot **
vol_snap_get_children(struct vol_snapshot *vsnap, int *err)
{
	struct vol_snapshot **children;
	if (!vsnap->numchildren) {
		*err = ERR_EMPTY;
		return NULL;
	}
	children = calloc(vsnap->numchildren, sizeof(struct vol_snapshot *));
	ASSERT(vsnap->numchildren <= 2);
	ASSERT(vsnap->snapid * 2 <= numsnaps);
	children[0] = &vsnaps[vsnap->snapid * 2]
	if (vsnap->numchildren > 1) {
		ASSERT(vsnap->snapid * 2  + 1 <= numsnaps);
		children[1] = &vsnaps[vsnap->snapid * 2 + 1];
	}
	return children;
}

int
vol_snap_get_all_descendants(struct vol_snapshot *vsnap, uint32_t **descendants,
			     uint32_t *ndescendants)
{

}
int
main()
{
}
