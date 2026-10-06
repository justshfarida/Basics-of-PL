import numpy as np
from PIL import Image

names = ["crop", "downsample", "flip_h", "flip_v", "negative_index"]

for name in names:
    a = np.array(Image.open(f"images/numpy_{name}.png"))
    b = np.array(Image.open(f"../cpp/images/cpp_{name}.png"))
    same = a.shape == b.shape and np.array_equal(a, b)
    print(f"{name:15s} numpy {a.shape}  c++ {b.shape}  identical: {same}")
