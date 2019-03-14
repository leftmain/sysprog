#include <ucontext.h>
#include <signal.h>
#include <time.h>
#include "arrays.h"

#define stack_size 1024 * 1024
#define TIME CLOCK_MONOTONIC
#define M 1000000000

#define handle_error(msg) \
   do { perror(msg); exit(EXIT_FAILURE); } while (0)

#define change_context() \
	do {	clock_gettime(TIME, &t2); \
			dt += M*(t2.tv_sec-t1.tv_sec)+(t2.tv_nsec-t1.tv_nsec); \
			if (swapcontext(uctx_func + I, uctx_func + (I+1) % N) == -1) \
				handle_error("swapcontext"); \
			clock_gettime(TIME, &t1); } while (0)

static ucontext_t uctx_main, * uctx_func;
static int N, SORTED;

void heap_sort(int *a, int n, int I) {
    int x = 0;
    int i = 0;
	int j = 0;
	struct timespec t1, t2;
	long int dt = 0;

//printf("heap_sort%d: started\n", I);
	clock_gettime(TIME, &t1);

    for (i = 1; i < n; i++) {
        x = a[i];
        j = i;
        while (j > 0) {
            if (x < a[(j - 1) / 2]) {
                a[j] = a[(j - 1) / 2];
                j = (j - 1) / 2;
            } else break;
        }
        a[j] = x;
    }
	change_context();
    
    for (i = n - 1; i > 0; i--) {
        x = a[i];
		
        a[i] = a[0];
        a[0] = x;
		j = 0;
		while (1) {
			if (2 * j + 2 < i) {
				if (a[2 * j + 1] < a[2 * j + 2]) {
					if (x > a[2 * j + 1]) {
						a[j] = a[2 * j + 1];
						j = 2 * j + 1;
					} else break;
				} else {
					if (x > a[2 * j + 2]) {
						a[j] = a[2 * j + 2];
						j = 2 * j + 2;
					} else break;
				}
			} else if (2 * j + 1 < i) {
				if (x > a[2 * j + 1]) {
					a[j] = a[2 * j + 1];
					j = 2 * j + 1;
				} else break;
			} else break;
		}
		a[j] = x;
	}
	clock_gettime(TIME, &t2);
	dt += M*(t2.tv_sec-t1.tv_sec)+(t2.tv_nsec-t1.tv_nsec);
	printf("time%d: %ld\n", I, dt/1000);

	SORTED++;
	while (SORTED < N)
		if (swapcontext(uctx_func + I, uctx_func + (I+1) % N) == -1)
			handle_error("swapcontext");
	if (swapcontext(uctx_func + I, &uctx_main) == -1)
		handle_error("swapcontext");
}

static void *
allocate_stack_sig()
{
	void *stack = malloc(stack_size);
	if (!stack) return 0;
	stack_t ss;
	ss.ss_sp = stack;
	ss.ss_size = stack_size;
	ss.ss_flags = 0;
	sigaltstack(&ss, NULL);
	return stack;
}

int main(int argc, char ** argv) {
	int ** a = 0;
	int i = 0;
	int j = 0;
	char ** func_stack = 0;
	const char * res_file = "result.txt";
	struct timespec t1, t2;
	double t = 0.;
	N = SORTED = 0;

	if (argc < 2) {
		printf("usage: %s [file1] [file2] ...\n", argv[0]);
		return 0;
	}

	uctx_func = (ucontext_t *)malloc(argc * sizeof(ucontext_t));
	if (!uctx_func) {
		printf("malloc error\n");
		return -1;
	}
	func_stack = (char **)malloc(argc * sizeof(char **));
	if (!func_stack) {
		printf("malloc error\n");
		free(uctx_func);
		return -1;
	}
	a = (int **)calloc((argc - 1), sizeof(int *));
	if (!a) {
		printf("calloc error\n");
		free(uctx_func);
		free(func_stack);
		return -1;
	}

	read_arrays(a, &N, argc, argv);

	clock_gettime(TIME, &t1);
	for (i = 0; i < N; i++) {
		if (!(func_stack[i] = allocate_stack_sig()) ||
			getcontext(uctx_func + i) == -1) {
			free(uctx_func);
			free(func_stack);
			for (j = 0; j < argc - 1; j++) if (a[j]) free(a[j]);
			free(a);
			return -2;
		}
		uctx_func[i].uc_stack.ss_sp = func_stack[i];
		uctx_func[i].uc_stack.ss_size = stack_size;
		uctx_func[i].uc_link = &uctx_main;
		makecontext(uctx_func + i, heap_sort, 3, a[i] + 1, *a[i], i);
	}
	if (swapcontext(&uctx_main, uctx_func) == -1)
		handle_error("swapcontext");
	clock_gettime(TIME, &t2);
	printf("time: %ld\n", 1000000*(t2.tv_sec-t1.tv_sec)+(t2.tv_nsec-t1.tv_nsec)/1000);

	merge(a, N, res_file);

	free(uctx_func);
	free(func_stack);
	for (i = 0; i < argc - 1; i++) if (a[i]) free(a[i]);
	free(a);
	return 0;
}


