#include <pthread.h>
#include <stdio.h>

int val;
int
main()
{
	pthread_rwlock_t rwlock;

	pthread_rwlock_init(&rwlock, NULL);

	pthread_rwlock_wrlock(&rwlock);
	pthread_rwlock_rdlock(&rwlock);
	val = 5;
	pthread_rwlock_unlock(&rwlock);
	pthread_rwlock_unlock(&rwlock);

	pthread_rwlock_rdlock(&rwlock);
	printf("val = %d\n", val);
	pthread_rwlock_unlock(&rwlock);

	return 0;
}
