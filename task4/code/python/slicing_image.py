import os

import numpy as np
from PIL import Image

os.makedirs("images", exist_ok=True)

# Load the photo as grayscale, so it is a 2D matrix of values 0-255.
# Both the Python and the C++ program start from this same input.png.
img = np.array(Image.open("../../image_cat.jpg").convert("L"))
Image.fromarray(img).save("images/input.png")
h, w = img.shape

cases = {
    "crop":           img[h // 4 : 3 * h // 4, w // 4 : 3 * w // 4],
    "downsample":     img[::4, ::4],
    "flip_h":         img[:, ::-1],
    "flip_v":         img[::-1, :],
    "negative_index": img[-(h // 2):, -(w // 2):],
}

print(f"input: {h} x {w}")
for name, result in cases.items():
    Image.fromarray(result).save(f"images/numpy_{name}.png")
    print(f"{name}: {result.shape[0]} x {result.shape[1]}")
