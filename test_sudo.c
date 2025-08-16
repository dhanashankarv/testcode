#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>

#define LOGINFO(...) fprintf(stdout, __VA_ARGS__)
#define LOGERR(...) fprintf(stderr, __VA_ARGS__)

int
main(int argc, char *argv[])
{
	int ret, i, debug;
	uid_t euid;
	if (argc < 2) {
		return 0;
	}
	euid = geteuid();
	/*
	 * Set the uid of this process to the euid so that any processes
	 * spawned by this process start with the euid.
	 */
	ret = setuid(euid);
	if (ret < 0) {
		ret = errno;
		LOGERR("Setuid(%d) failed: %s (%d)", euid, strerror(ret), ret);
		return ret;
	}
	debug = secure_getenv("DEBUG");
	if (debug) {
		LOGINFO("File %s has euid %d\n", argv[0], euid);
		LOGINFO("Input args\n");
		for (i = 1; i < argc; i++) {
			LOGINFO("%d: %p %s\n", i, &argv[i], argv[i]);
		}
		LOGINFO("Calling execvp(%s, %p)\n", argv[1], argv + 2);
	}
	execvp(argv[1], argv + 1);

	/* NOT REACHED */
	return 0;
}
