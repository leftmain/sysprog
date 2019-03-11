#include "arrays.h"

void read_arrays(int ** a, int * N, int argc, char ** argv) {
	FILE * fp = 0;
	int i = 0, x = 0, n = 0;
	for (i = 0; i < argc - 1; i++) {
		n = 0;
		if (!(fp = fopen(argv[i + 1], "r"))) {
			printf("cannot open %s\n", argv[i + 1]);
			a[i] = 0;
			continue;
		}
		while (fscanf(fp, "%d", &x) == 1) n++;
		if (!feof(fp)) {
			printf("cannot read %s\n", argv[i + 1]);
			a[i] = 0;
			fclose(fp);
			argc--; i--;
			continue;
		}
		rewind(fp);
		a[i] = (int *)calloc((n + 1), sizeof(int));
		if (!a[i]) {
			printf("memory error on %d array (file %s)\n", i, argv[i+1]);
			fclose(fp);
			argc--; i--;
			continue;
		}
		*a[i] = n;
		for (x = 1; x <= n; x++)
			fscanf(fp, "%d ", a[i] + x);
		fclose(fp);
	}
	*N = argc - 1;
}

void print_arrays(int ** a, int argc, char ** argv) {
	FILE * fp = 0;
	int i = 0, x = 0;
	for (i = 0; i < argc - 1; i++) {
		if (! *a[i]) continue;
		if (!(fp = fopen(argv[i + 1], "w"))) {
			printf("cannot open %s\n", argv[i + 1]);
			continue;
		}
		for (int x = 1; x <= *a[i]; x++) fprintf(fp, "%d ", a[i][x]);
		printf("file %s sorted\n", argv[i + 1]);
		fclose(fp);
	}
}

void lin_sort(int * a, int n) {
	int i = 1, j = 0, x = 0;
	for (; i < n; i++) {
		x = a[i];
		j = i - 1;
		while (j >= 0 && a[j] < x) {
			a[j + 1] = a[j];
			j--;
		}
		a[j + 1] = x;
	}
}

void merge(int ** a, int N, const char * file) {
	int i = 0;
	int mini = 0;
	int min = a[0][1];
	FILE * fp;
	if (!(fp = fopen(file, "w"))) {
		printf("cannot open %s\n", file);
		return;
	}
	while (1) {
		for (i = 0; i < N && ! *a[i]; i++);
		if (i >= N) break;
		mini = i;
		min = a[i][a[i][0]];
		for (i = 0; i < N; i++) {
			if (*a[i] && a[i][*a[i]] < min) {
				min = a[i][*a[i]];
				mini = i;
			}
		}
		fprintf(fp, "%d ", min);
		a[mini][0]--;
	}
	fclose(fp);
}

