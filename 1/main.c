#include <ucontext.h>
#include "arrays.h"

#define stack_size 1024 * 1024

//static ucontext_t * uctx;

int main(int argc, char ** argv) {
	int ** a = 0;
	int N = 0;
	int i = 0;
	const char * res_file = "result.txt";

	if (argc < 2) {
		printf("usage: %s [file1] [file2] ...\n", argv[0]);
		return 0;
	}

/*
	uctx = (ucontext_t *)malloc(argc * sizeof(ucontext_t));
	if (!uctx) {
		printf("malloc error\n");
		return -1;
	}
*/

	a = (int **)calloc((argc - 1), sizeof(int *));
	if (!a) {
		printf("calloc error\n");
		return -1;
	}

	read_arrays(a, &N, argc, argv);
/*
	for (i = 0; i < N; i++) {
		for (int j = 1; j <= a[i][0]; j++) printf("%d ", a[i][j]);
		printf("\n");
	}
*/
	for (i = 0; i < N; i++) lin_sort(a[i] + 1, *a[i]);
	print_arrays(a, argc, argv);
	merge(a, N, res_file);

	for (i = 0; i < argc - 1; i++) if (a[i]) free(a[i]);
	free(a);
	return 0;
}


