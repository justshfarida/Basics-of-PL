"""2D matrix slicing with NumPy.

Run:
    python slicing_numpy.py
"""

import numpy as np

# Each case: (name, row slice, column slice) in Python's start:stop:step form.
# None means "use the default", exactly like leaving it out in m[::].
CASES = [
    ("crop",           slice(1, 4, 1),        slice(2, 5, 1)),
    ("every_second",   slice(None, None, 2),  slice(None, None, 2)),
    ("flip_h",         slice(None, None, 1),  slice(None, None, -1)),
    ("flip_v",         slice(None, None, -1), slice(None, None, 1)),
    ("negative_index", slice(-2, None, 1),    slice(-3, None, 1)),
    ("mixed",          slice(4, 0, -2),       slice(1, None, 2)),
]


def slice_text(s):
    """Show a slice object the way it is written in code, e.g. '1:4:1'."""
    part = lambda v: "" if v is None else str(v)
    return f"{part(s.start)}:{part(s.stop)}:{part(s.step)}"


def print_matrix(m):
    for row in m:
        print("".join(f"{v:4d}" for v in row))


def main():
    rows, cols = 5, 6
    m = 10 * np.arange(rows).reshape(-1, 1) + np.arange(cols)

    print(f"original ({rows} x {cols})")
    print_matrix(m)

    for name, rs, cs in CASES:
        result = m[rs, cs]
        print()
        print(f"{name}: m[{slice_text(rs)}, {slice_text(cs)}] "
              f"-> {result.shape[0]} x {result.shape[1]}")
        print_matrix(result)


if __name__ == "__main__":
    main()
