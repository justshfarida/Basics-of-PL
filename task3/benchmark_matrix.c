/*
 * Execution-time benchmark for matrix_mul().
 * Build and run:
 *   gcc -std=c99 -Wall -O2 benchmark_matrix.c matrix.c -o benchmark_matrix.exe
 *   .\benchmark_matrix.exe
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "matrix.h"

#define MIN_TOTAL_SECONDS 0.5   /* repeat small sizes until this much time passes */

static void fill_random(double *m, size_t n)
{
    for (size_t i = 0; i < n; i++) {
        m[i] = (double)rand() / RAND_MAX;   /* value in [0, 1] */
    }
}

int main(void)
{
    const size_t sizes[] = {64, 128, 256, 512, 1024};
    const int num_sizes = sizeof(sizes) / sizeof(sizes[0]);

    srand(42);

    for (int s = 0; s < num_sizes; s++) {
        size_t n = sizes[s];
        double *a = malloc(n * n * sizeof(double));
        double *b = malloc(n * n * sizeof(double));
        double *c = malloc(n * n * sizeof(double));
        if (a == NULL || b == NULL || c == NULL) {
            printf("Out of memory for N = %zu\n", n);
            return 1;
        }

        fill_random(a, n * n);
        fill_random(b, n * n);

        /* Repeat until enough time has passed for a stable average. */
        int runs = 0;
        clock_t start = clock();
        double elapsed;
        do {
            matrix_mul(a, b, c, n, n, n);
            runs++;
            elapsed = (double)(clock() - start) / CLOCKS_PER_SEC;
        } while (elapsed < MIN_TOTAL_SECONDS);

        double ms_per_run = elapsed / runs * 1000.0;
        printf("%zu %d %f\n", n, runs, ms_per_run);

        free(a);
        free(b);
        free(c);
    }
    return 0;
}