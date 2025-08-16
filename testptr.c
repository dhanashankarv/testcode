#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
char *testnames[] = {"dhana", "shankar", "vikasini", "inbha" };

void
fltinj_point_hashiter(void *iter_names, char *l)
{
	char *name = strdup(l);
	char ***iter = iter_names;
	**iter = name;
	(*iter)++;
}

int
fltinj_point_list(char ***fp_names, int *nfp_names)
{
	int i;
	int cnt = sizeof(testnames)/sizeof(testnames[0]);
	char **names, ***iter;
	*nfp_names = 0;
	*fp_names = NULL;
	names = malloc(sizeof(char *) * cnt);
	if (!names) {
		return ENOMEM;
	}
	*fp_names = names;
	*nfp_names = cnt;
	iter = &names;
	for (i = 0; i < cnt; i++) {
		fltinj_point_hashiter(iter, testnames[i]);
	}
	return 0;
}

int
main(int argc, char *argv[])
{
	int i;
	char **lnames;
	int nlnames;

	fltinj_point_list(&lnames, &nlnames);

	for (i = 0; i < nlnames; i++) {
		printf("%s ", lnames[i]);
	}
	printf("\n");
	for (i = 0; i < nlnames; i++) {
		free(lnames[i]);
	}
	free(lnames);
	return 0;
}
