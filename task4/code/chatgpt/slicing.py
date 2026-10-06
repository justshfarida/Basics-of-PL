import numpy as np

# Create a 5x5 matrix
matrix = np.array([
    [1,  2,  3,  4,  5],
    [6,  7,  8,  9, 10],
    [11, 12, 13, 14, 15],
    [16, 17, 18, 19, 20],
    [21, 22, 23, 24, 25]
])

# Slice: rows 1-3 and columns 1-3
sliced = matrix[1:4, 1:4]

print("Original matrix:")
print(matrix)

print("\nSliced matrix:")
print(sliced)
