import numpy as np


def matrix_mul(a: np.ndarray, b: np.ndarray) -> np.ndarray:
    if a.ndim != 2 or b.ndim != 2:
        raise ValueError("Both inputs must be 2D matrices")
    if a.shape[1] != b.shape[0]:
        raise ValueError(
            f"Incompatible shapes: {a.shape} and {b.shape}"
        )
    return a @ b


a = np.array([[1, 2, 3],
                  [4, 5, 6]])
b = np.array([[1, 2],
                  [3, 4],
                  [5, 6]])

print("A =")
print(a)
print("B =")
print(b)
print("A x B =")
print(matrix_mul(a, b))