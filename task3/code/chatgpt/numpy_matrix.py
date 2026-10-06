import time
import numpy as np


def matrix_multiply(A, B):
    return A @ B


def benchmark():
    sizes = [100, 200, 500, 1000]

    print("NumPy Matrix Multiplication")
    print("---------------------------")

    for n in sizes:
        A = np.random.rand(n, n)
        B = np.random.rand(n, n)

        # Warm-up
        matrix_multiply(A, B)

        start = time.perf_counter()
        C = matrix_multiply(A, B)
        end = time.perf_counter()

        print(f"Matrix size: {n}x{n}")
        print(f"Time: {end - start:.6f} seconds")
        print()


def test_correctness():
    A = np.array([
        [1, 2],
        [3, 4]
    ])

    B = np.array([
        [5, 6],
        [7, 8]
    ])

    expected = np.array([
        [19, 22],
        [43, 50]
    ])

    result = matrix_multiply(A, B)

    assert np.array_equal(result, expected)

    print("NumPy correctness test passed!")


if __name__ == "__main__":
    test_correctness()
    benchmark()
