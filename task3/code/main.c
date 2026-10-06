#include <stdio.h>
#include "matrix.h"

static void print_matrix(const char *name, const double *m,
                         size_t rows, size_t cols)
{
    printf("%s =\n", name);
    for (size_t i = 0; i < rows; i++) {
        for (size_t j = 0; j < cols; j++) {
            printf("%6.0f", m[i * cols + j]);
        }
        printf("\n");
    }
}

int main(void)
{
    double a[2 * 3] = {1, 2, 3,
                       4, 5, 6};
    double b[3 * 2] = {1, 2,
                       3, 4,
                       5, 6};
    double c[2 * 2];

    matrix_mul(a, b, c, 2, 3, 2);

    print_matrix("A", a, 2, 3);
    print_matrix("B", b, 3, 2);
    print_matrix("A x B", c, 2, 2);
    return 0;
}