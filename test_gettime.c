#include <stdio.h>
#include <time.h>

int
gettime(struct timespec *time)
{
        int ret;
        ret = clock_gettime(CLOCK_MONOTONIC_RAW, time);
        if (ret < 0) {
                perror("clock_gettime(CLOCK_PROCESS_CPUTIME_ID) failed");
                return ret;
        }
        return ret;
}

int
main()
{
        int ret = 0;
        struct timespec start, end, sleep = {5, 0};
        ret = gettime(&start);
        if (ret < 0) {
                return ret;
        }
        printf("%ld.%ld\n", start.tv_sec, start.tv_nsec);
        nanosleep(&sleep, NULL);
        ret = gettime(&end);
        if (ret < 0) {
                return ret;
        }
        printf("%ld.%ld\n", end.tv_sec, end.tv_nsec);
        return ret;
}
