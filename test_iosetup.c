#include <libaio.h>
#include <stdio.h>

int
main()
{
        io_context_t ctx;
        int ret;
        int nr_events = 2048;

        while(1) {
                ret = io_setup(nr_events, &ctx);
                if (ret < 0) {
                        fprintf(stderr, "io_setup(%d) failed: %s\n", nr_events, strerror(-ret));
                        nr_events /= 2;
                } else {
                        fprintf(stdout, "io_setup(%d) succeeded\n", nr_events);
                        break;
                }
                if (nr_events == 1) {
                        break;
                }
        }
        if (ctx) {
                io_destroy(ctx);
        }
	return 0;
}
