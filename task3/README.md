# Task 3: Matrix Multiplication in NumPy and C

## 1. Objective

This task implements matrix multiplication in two ways and compares them:

- **Python with NumPy**, using the built-in `@` operator.
- **C**, using a hand-written triple loop.

The C implementation is verified with unit tests. Both implementations are then compared in terms of **code size** and **execution time**.

## 2. Environment

| Item | Value |
|---|---|
| OS | Windows (64-bit) |
| CPU | Intel Core Ultra 9 285H (16 cores) |
| C compiler | GCC 15.2.0 (MinGW-w64), flags `-std=c99 -O2` |
| Python | 3.14.7 |
| NumPy | 2.5.3 |

All benchmarks were run on the same machine, from the command line.

## 3. Files

All source files are in the `code/` folder.

| File | Purpose |
|---|---|
| `matrix_multiplication.py` | NumPy implementation and demo |
| `benchmark_matrix.py` | Execution time benchmark for NumPy |
| `matrix.h` | Declaration of the C function `matrix_mul` |
| `matrix.c` | C implementation (triple loop) |
| `main.c` | C demo program |
| `test_matrix.c` | Unit tests for the C implementation |
| `benchmark_matrix.c` | Execution time benchmark for C |

### Build and run

From the `code/` folder:

```powershell
# C demo
gcc -std=c99 -Wall -O2 main.c matrix.c -o matmul_demo.exe
.\matmul_demo.exe

# C unit tests
gcc -std=c99 -Wall -O2 test_matrix.c matrix.c -o test_matrix.exe
.\test_matrix.exe

# C benchmark
gcc -std=c99 -Wall -O2 benchmark_matrix.c matrix.c -o benchmark_matrix.exe
.\benchmark_matrix.exe

# NumPy demo and benchmark
python matrix_multiplication.py
python benchmark_matrix.py
```

## 4. Implementation

### 4.1 NumPy

```python
def matrix_mul(a: np.ndarray, b: np.ndarray) -> np.ndarray:
    if a.ndim != 2 or b.ndim != 2:
        raise ValueError("Both inputs must be 2D matrices")
    if a.shape[1] != b.shape[0]:
        raise ValueError(f"Incompatible shapes: {a.shape} and {b.shape}")
    return a @ b
```

The multiplication itself is a single operator, `a @ b`. The rest of the function only validates the input shapes. Internally, NumPy hands the work to an optimized linear algebra library (BLAS).

### 4.2 C

```c
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
```

Each element of the result is the dot product of a row of A and a column of B, computed with the classic triple loop. Matrices are stored as flat 1D arrays in **row-major order**: element `(i, j)` of a matrix with `cols` columns is at index `i * cols + j`. This is the same memory layout NumPy uses by default, and it lets the function handle matrices of any size. The caller allocates the result matrix `c`. The function returns `-1` if any pointer is `NULL`.

The function is declared in `matrix.h` and defined in `matrix.c`, separate from any `main()`. This lets the demo, the unit tests and the benchmark each have their own `main()` while all using the same implementation.

## 5. Unit Tests

`test_matrix.c` contains 10 tests. Each one calls `matrix_mul` and compares the result with a value calculated by hand.

| Test | What it checks |
|---|---|
| 2×3 × 3×2 | Basic rectangular case (result `[[22, 28], [49, 64]]`) |
| 2×2 × 2×2 | Simple square case |
| A × I = I × A = A | Multiplying by the identity matrix, from both sides |
| A × 0 = 0 | Result is fully overwritten, even if `c` held old values |
| 1×1 × 1×1 | Smallest possible case |
| 1×3 × 3×1 | Row times column gives a 1×1 dot product |
| 3×1 × 1×3 | Column times row gives a 3×3 outer product |
| Negative and fractional values | Correct signs and non-integer values |
| AB ≠ BA | The implementation does not wrongly treat multiplication as commutative |
| NULL pointers | Invalid input returns `-1` instead of crashing |

Results are compared with exact equality. This is safe here because every test value is an integer or a half (such as 0.5), which binary floating point represents exactly, so no rounding error can occur.

Result:

```
10 tests run, 10 passed, 0 failed
```

## 6. Code Size Analysis

### 6.1 Lines of code

Counted without blank lines and comments:

| Part | NumPy | C |
|---|---|---|
| Multiplication logic | 1 (`a @ b`) | 9 (triple loop) |
| Whole function including input checks | 8 | 18 (`matrix.c`) + 6 (`matrix.h`) |
| Demo program | 20 (`matrix_multiplication.py`, including the function) | 27 (`main.c`) |
| Unit tests | none | 162 (`test_matrix.c`) |
| Benchmark | 19 (`benchmark_matrix.py`) | 43 (`benchmark_matrix.c`) |

The NumPy version needs **one operator** for the actual multiplication. In C, the programmer must write the loops, compute the index of every element manually, and handle memory allocation in the calling code.

### 6.2 Compiled and installed size

| Measure | Size |
|---|---|
| C demo executable `matmul_demo.exe` | 53.4 KB (54,682 bytes) |
| NumPy package (installed) | 31.3 MB |

The two sizes describe opposite trade-offs. The C program is small and self-contained, but every line of its algorithm was written by hand. The NumPy version is tiny to write, but it depends on a large installed package, which bundles a highly optimized BLAS library.

## 7. Execution Time

### 7.1 Method

Both benchmarks multiply two random N×N matrices with values between 0 and 1, for N = 64, 128, 256, 512 and 1024. The random generators use a fixed seed (42), so every run uses the same matrices.

A single small multiplication finishes too quickly to time reliably, so each size is repeated until at least 0.5 seconds have passed, and the total time is divided by the number of repetitions:

```
time per multiplication = total time / runs
```

C uses `clock()` and Python uses `time.perf_counter()`.

NumPy was run with its default settings, so its BLAS library may use **several CPU cores**, while the C implementation always uses **one core**.

### 7.2 Results

| N | C (ms) | NumPy (ms) | NumPy faster by |
|---|---|---|---|
| 64 | 0.080 | 0.016 | 5× |
| 128 | 0.906 | 0.108 | 8× |
| 256 | 15.088 | 0.378 | 40× |
| 512 | 138.000 | 1.551 | 89× |
| 1024 | 2829.000 | 9.561 | 296× |

