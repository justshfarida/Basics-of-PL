"""Execution-time benchmark for NumPy matrix multiplication.
Run:
    python benchmark_numpy.py
"""

import time

import numpy as np

SIZES = [64, 128, 256, 512, 1024]
MIN_TOTAL_SECONDS = 0.5   #repeat small sizes until this much time passes


def time_matmul(a, b):
    runs = 0
    start = time.perf_counter()
    while True:
        a @ b
        runs += 1
        elapsed = time.perf_counter() - start
        if elapsed >= MIN_TOTAL_SECONDS:
            return runs, elapsed / runs * 1000


rng = np.random.default_rng(42)
for n in SIZES:
    a = rng.random((n, n))
    b = rng.random((n, n))
    runs, ms = time_matmul(a, b)
    print(f"{n} {runs} {ms}")