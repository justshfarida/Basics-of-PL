#include "matrix.h"
 
int matrix_mul(const double *a, const double *b, double *c,
           size_t rows_a, size_t cols_a, size_t cols_b)
{
    if (a == NULL || b == NULL || c == NULL) {
        return -1;
    }
 
    for (size_t i = 0; i < rows_a; i++) {
        for (size_t j = 0; j < cols_b; j++) {
            double sum = 0.0;
            for (size_t k = 0; k < cols_a; k++) {
                sum += a[i * cols_a + k] * b[k * cols_b + j];
            }
            c[i * cols_b + j] = sum;
        }
    }
    return 0;
}
 