#include <stdio.h>
#include <stddef.h>

int matmul(const double *a, const double *b, double *c,
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

static void print_matrix(const char *name, const double *m,
                         size_t rows, size_t cols)
{
    printf("%s =\n", name);
    for (size_t i = 0; i < rows; i++) {
        for (size_t j = 0; j < cols; j++) {
            printf("%f", m[i * cols + j]);
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

    matmul(a, b, c, 2, 3, 2);

    print_matrix("A", a, 2, 3);
    print_matrix("B", b, 3, 2);
    print_matrix("A x B", c, 2, 2);
    return 0;
}