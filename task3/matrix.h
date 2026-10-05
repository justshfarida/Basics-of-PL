#ifndef MATRIX_H
#define MATRIX_H
 
#include <stddef.h>
 
/*
 * Multiplies matrix A (rows_a x cols_a) by matrix B (cols_a x cols_b)
 * and stores the result in C (rows_a x cols_b).
 *
 * All matrices are stored in row-major order as 1D arrays:
 * element (i, j) of a matrix with `cols` columns is at index i * cols + j.
 *
 * Returns 0 on success, -1 if any pointer is NULL.
 */
int matrix_mul(const double *a, const double *b, double *c,
           size_t rows_a, size_t cols_a, size_t cols_b);
 
#endif
 