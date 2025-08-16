#include <stdio.h>
typedef unsigned int	uint32_t;
typedef struct {
	volatile int		counter;
} cpr_atomic_t;

#define cpr_atomic_get(a)	((a)->counter)
#define cpr_atomic_inc(a)	(uint32_t)__sync_add_and_fetch(&(a)->counter, 1)
union {
	cpr_atomic_t	val;
	struct {
		uint32_t updating: 4,
			 poffset : 22,
			 compress: 3,
			 encrypt : 3;
	};
} be;

int
main()
{
	be.updating = 1;
	be.poffset  = 1024;
	be.compress = 1;
	be.encrypt  = 2;
	printf("val: 0x%x updating: 0x%x poffset: 0x%x (%d) compress: %d encrypt: %d\n",
		cpr_atomic_get(&be.val), be.updating, be.poffset, be.poffset, be.compress,
		be.encrypt);
	cpr_atomic_inc(&be.val);
	printf("val: 0x%x updating: 0x%x poffset: 0x%x (%d) compress: %d encrypt: %d\n",
		cpr_atomic_get(&be.val), be.updating, be.poffset, be.poffset, be.compress,
		be.encrypt);

}
