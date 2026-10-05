/*
 * Unit tests for matrix_mul().
 *
 * Build and run:
 *   gcc -std=c99 -Wall -O2 test_matrix.c matrix.c -o test_matrix.exe
 *   .\test_matrix.exe
 *
 * The program prints PASS/FAIL for every test and returns 0 only if
 * all tests pass, so it can also be used in scripts.
 */

#include <stdio.h>
#include "matrix.h"

static int tests_run = 0;
static int tests_failed = 0;

/*
 * Returns 1 if two matrices of n elements are exactly equal..
 */
static int matrices_equal(const double *x, const double *y, size_t n)
{
    for (size_t i = 0; i < n; i++) {
        if (x[i] != y[i]) {
            printf("    mismatch at index %zu: got %g, expected %g\n",
                   i, x[i], y[i]);
            return 0;
        }
    }
    return 1;
}

static void check(int condition, const char *name)
{
    tests_run++;
    if (condition) {
        printf("[PASS] %s\n", name);
    } else {
        tests_failed++;
        printf("[FAIL] %s\n", name);
    }
}
static void test_rectangular_2x3_times_3x2(void)
{
    double a[] = {1, 2, 3,
                  4, 5, 6};
    double b[] = {1, 2,
                  3, 4,
                  5, 6};
    double expected[] = {22, 28,
                         49, 64};
    double c[4];

    int status = matrix_mul(a, b, c, 2, 3, 2);
    check(status == 0 && matrices_equal(c, expected, 4),
          "2x3 times 3x2 gives correct 2x2 result");
}

static void test_square_2x2(void)
{
    double a[] = {1, 2,
                  3, 4};
    double b[] = {5, 6,
                  7, 8};
    double expected[] = {19, 22,
                         43, 50};
    double c[4];

    matrix_mul(a, b, c, 2, 2, 2);
    check(matrices_equal(c, expected, 4), "2x2 times 2x2");
}

static void test_identity(void)
{
    double a[] = {1, 2, 3,
                  4, 5, 6,
                  7, 8, 9};
    double id[] = {1, 0, 0,
                   0, 1, 0,
                   0, 0, 1};
    double c[9];

    matrix_mul(a, id, c, 3, 3, 3);
    int right = matrices_equal(c, a, 9);
    matrix_mul(id, a, c, 3, 3, 3);
    int left = matrices_equal(c, a, 9);

    check(right && left, "A x I = I x A = A");
}

static void test_zero_matrix(void)
{
    double a[] = {1, 2,
                  3, 4};
    double zero[] = {0, 0,
                     0, 0};
    double c[] = {99, 99, 99, 99};   /* garbage that must be overwritten */

    matrix_mul(a, zero, c, 2, 2, 2);
    check(matrices_equal(c, zero, 4), "A x 0 = 0 (and old values are overwritten)");
}

static void test_1x1(void)
{
    double a[] = {3};
    double b[] = {4};
    double c[1];

    matrix_mul(a, b, c, 1, 1, 1);
    check(c[0] == 12, "1x1 times 1x1");
}

/* Row vector times column vector = dot product (1x1). */
static void test_dot_product(void)
{
    double row[] = {1, 2, 3};
    double col[] = {4,
                    5,
                    6};
    double c[1];

    matrix_mul(row, col, c, 1, 3, 1);
    check(c[0] == 32, "1x3 times 3x1 gives dot product (1x1)");
}

/* Column vector times row vector = outer product (3x3). */
static void test_outer_product(void)
{
    double col[] = {1,
                    2,
                    3};
    double row[] = {4, 5, 6};
    double expected[] = { 4,  5,  6,
                          8, 10, 12,
                         12, 15, 18};
    double c[9];

    matrix_mul(col, row, c, 3, 1, 3);
    check(matrices_equal(c, expected, 9), "3x1 times 1x3 gives outer product (3x3)");
}

static void test_negative_and_fractional(void)
{
    double a[] = {-1.5,  2.0,
                   0.5, -3.0};
    double b[] = { 2.0, -1.0,
                   4.0,  0.5};
    double expected[] = { 5.0,  2.5,
                        -11.0, -2.0};
    double c[4];

    matrix_mul(a, b, c, 2, 2, 2);
    check(matrices_equal(c, expected, 4), "negative and fractional values");
}

/* Matrix multiplication is generally NOT commutative: AB != BA. */
static void test_not_commutative(void)
{
    double a[] = {1, 2,
                  3, 4};
    double b[] = {0, 1,
                  1, 0};
    double ab[4], ba[4];

    matrix_mul(a, b, ab, 2, 2, 2);
    matrix_mul(b, a, ba, 2, 2, 2);

    int differ = 0;
    for (int i = 0; i < 4; i++) {
        if (ab[i] != ba[i]) {
            differ = 1;
        }
    }
    check(differ, "AB != BA for non-commuting matrices");
}

static void test_null_pointers(void)
{
    double a[] = {1};
    double c[1];

    int r1 = matrix_mul(NULL, a, c, 1, 1, 1);
    int r2 = matrix_mul(a, NULL, c, 1, 1, 1);
    int r3 = matrix_mul(a, a, NULL, 1, 1, 1);

    check(r1 == -1 && r2 == -1 && r3 == -1, "NULL pointers return -1");
}


int main(void)
{
    printf("Running matrix_mul unit tests\n\n");

    test_rectangular_2x3_times_3x2();
    test_square_2x2();
    test_identity();
    test_zero_matrix();
    test_1x1();
    test_dot_product();
    test_outer_product();
    test_negative_and_fractional();
    test_not_commutative();
    test_null_pointers();

    printf("\n%d tests run, %d passed, %d failed\n",
           tests_run, tests_run - tests_failed, tests_failed);

    return tests_failed == 0 ? 0 : 1;
}