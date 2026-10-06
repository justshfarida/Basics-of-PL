# Task 4: 2D Matrix Slicing in NumPy and C++

## 1. Objective

This task implements 2D matrix slicing in two ways:

- **Python with NumPy**, using its built-in `m[rows, cols]` slicing syntax.
- **C++**, where slicing does not exist and is implemented by hand.

Both implementations are first tested on a small numeric matrix, then applied to a real photo. A grayscale image is a 2D matrix of pixel values, so every slice becomes a visible image operation: cropping, shrinking or flipping. Showing the NumPy and C++ results side by side makes it easy to see whether both solutions give the same result, and a pixel-by-pixel check confirms it.

## 2. Environment

| Item | Value |
|---|---|
| OS | Windows (64-bit) |
| C++ compiler | GCC 15.2.0 (MinGW-w64), flags `-std=c++17 -O2` |
| Python | 3.14.7 |
| Libraries | NumPy 2.5.3, Pillow (image loading and saving) |
| C++ image library | [stb_image](https://github.com/nothings/stb) by Sean Barrett (public domain) |

## 3. Files

```
task4/
├── image_cat.jpg              input photo
└── code/
    ├── python/
    │   ├── slicing_numpy.py   NumPy slicing on a small 5×6 matrix
    │   ├── slicing_image.py   converts the photo to grayscale and slices it with NumPy
    │   ├── compare.py         checks that the NumPy and C++ result images are identical
    │   └── images/            input.png and numpy_*.png
    ├── cpp/
    │   ├── slicing.h          C++ slicing interface (Slice, indices, slice2d)
    │   ├── slicing.cpp        C++ slicing implementation
    │   ├── main.cpp           C++ slicing on the same 5×6 matrix
    │   ├── slicing_image.cpp  slices the same grayscale image in C++
    │   ├── stb_image.h        external library, only used to read PNG files
    │   ├── stb_image_write.h  external library, only used to write PNG files
    │   ├── stb_impl.cpp       compiles the stb library
    │   └── images/            cpp_*.png
    └── chatgpt/               ChatGPT's solution (see section 10)
        ├── slicing.py
        └── slicing.c
```

### Build and run

Python, from `code/python/`:

```powershell
python slicing_numpy.py
python slicing_image.py      # creates images/input.png, needed by the C++ program
```

C++, from `code/cpp/`:

```powershell
g++ -std=c++17 -Wall -O2 main.cpp slicing.cpp -o slicing.exe
.\slicing.exe

g++ -std=c++17 -Wall -O2 slicing_image.cpp slicing.cpp stb_impl.cpp -o slicing_image.exe
.\slicing_image.exe
```

Comparison, from `code/python/`:

```powershell
python compare.py
```

## 4. How Slicing Works

A slice selects a range of indices using the form `start:stop:step`:

- **start** is the first index taken (included),
- **stop** is where to stop (**not** included),
- **step** is how far to jump each time.

Any part can be left out: a missing start or stop means "from the beginning" or "to the end". Two rules make slicing more powerful than a simple range:

- **Negative indices count from the end:** `-1` is the last element, `-2` the second-to-last.
- **A negative step walks backwards:** `::-1` reverses the order. With a negative step, the defaults swap: a missing start means "from the end" and a missing stop means "to the beginning".

Out-of-range values never cause an error; they are clamped to the valid range.

For a 2D matrix, one slice is given per dimension, separated by a comma: `m[rows, cols]`. For example, `m[1:4, 2:5]` takes rows 1–3 and columns 2–4.

## 5. Implementation

### 5.1 NumPy

Slicing is built into the language. Each case is a single expression:

```python
m[1:4, 2:5]      # crop
m[:, ::-1]       # mirror left to right
```

NumPy does not even copy the data. The result is a **view**: a new way of reading the same memory with a different start position and step.

### 5.2 C++

C++ has no slicing syntax, so the implementation needs three parts.

**A slice type.** `std::optional` represents a missing start or stop, just like `None` in Python:

```cpp
struct Slice {
    std::optional<long> start, stop;
    long step = 1;
};
```

**Converting a slice into indices.** `indices()` applies Python's rules for one dimension of length `n`:

```cpp
std::vector<long> indices(Slice s, long n)
{
    bool back = s.step < 0;
    long lo = back ? -1 : 0;
    long hi = back ? n - 1 : n;

    long start = back ? n - 1 : 0;
    long stop = back ? -1 : n;
    if (s.start) start = std::clamp(*s.start < 0 ? *s.start + n : *s.start, lo, hi);
    if (s.stop)  stop  = std::clamp(*s.stop  < 0 ? *s.stop  + n : *s.stop,  lo, hi);

    std::vector<long> result;
    for (long i = start; back ? i > stop : i < stop; i += s.step) {
        result.push_back(i);
    }
    return result;
}
```

1. If start or stop is missing, it gets the default for the direction of the step.
2. A negative value has `n` added to it, so it counts from the end.
3. The value is clamped into the allowed range. For a backwards step, that range is shifted down by one, because `-1` then means "before the first element".
4. A loop walks from start towards stop in steps of `step` and collects the indices.

**Building the 2D result.** `slice2d()` computes the selected row indices and column indices, then copies those elements into a new matrix:

```cpp
for (long r : indices(rows, n_rows)) {
    std::vector<int> row;
    for (long c : indices(cols, n_cols)) {
        row.push_back(m[r][c]);
    }
    result.push_back(row);
}
```

Unlike NumPy, this version copies every selected element into new memory.

## 6. Results on a Small Matrix

The test matrix is 5×6, and each value is `10 × row + column`. A value therefore shows where it came from: `23` is the element at row 2, column 3.

```
   0   1   2   3   4   5
  10  11  12  13  14  15
  20  21  22  23  24  25
  30  31  32  33  34  35
  40  41  42  43  44  45
```

| Slice | Result size | Result |
|---|---|---|
| `m[1:4, 2:5]` | 3 × 3 | `12 13 14` / `22 23 24` / `32 33 34` |
| `m[::2, ::2]` | 3 × 3 | `0 2 4` / `20 22 24` / `40 42 44` |
| `m[:, ::-1]` | 5 × 6 | every row reversed, starting `5 4 3 2 1 0` |
| `m[::-1, :]` | 5 × 6 | rows in reverse order, starting `40 41 42 43 44 45` |
| `m[-2:, -3:]` | 2 × 3 | `33 34 35` / `43 44 45` |

`slicing_numpy.py` and `main.cpp` print exactly the same values for every case.

## 7. Results on an Image

### 7.1 Input

The photo is first converted to **grayscale** by `slicing_image.py`, so it becomes a 2D matrix of 447 × 447 values from 0 (black) to 255 (white). It is saved as `code/python/images/input.png`, and **both** programs read this same file.

This matters because Python and C++ libraries decode JPEG files and convert color to gray with slightly different formulas. Starting both programs from separate copies of the original JPEG could make a few pixels differ by 1, even though both slicing implementations are correct.

| Original | Grayscale input (447 × 447) |
|---|---|
| <img src="image_cat.jpg" width="220"> | <img src="code/python/images/input.png" width="220"> |

### 7.2 NumPy and C++ side by side

The slices are defined relative to the image height `h` and width `w`, so they work for any image size.

| Case | Slice | Size | NumPy | C++ |
|---|---|---|---|---|
| Crop (center) | `[h//4 : 3h//4, w//4 : 3w//4]` | 224 × 224 | <img src="code/python/images/numpy_crop.png" width="160"> | <img src="code/cpp/images/cpp_crop.png" width="160"> |
| Downsample | `[::4, ::4]` | 112 × 112 | <img src="code/python/images/numpy_downsample.png" width="112"> | <img src="code/cpp/images/cpp_downsample.png" width="112"> |
| Flip horizontal | `[:, ::-1]` | 447 × 447 | <img src="code/python/images/numpy_flip_h.png" width="160"> | <img src="code/cpp/images/cpp_flip_h.png" width="160"> |
| Flip vertical | `[::-1, :]` | 447 × 447 | <img src="code/python/images/numpy_flip_v.png" width="160"> | <img src="code/cpp/images/cpp_flip_v.png" width="160"> |
| Negative index | `[-(h//2):, -(w//2):]` | 223 × 223 | <img src="code/python/images/numpy_negative_index.png" width="160"> | <img src="code/cpp/images/cpp_negative_index.png" width="160"> |

What each slice does to the picture:

- **Crop** keeps the middle half in both directions: the cat's face.
- **Downsample** keeps every 4th row and column, producing a picture 4 times smaller in each direction.
- **Flip horizontal** reverses the column order, mirroring the cat left to right.
- **Flip vertical** reverses the row order, turning the cat upside down.
- **Negative index** counts from the end and keeps the bottom-right quarter.

### 7.3 Verification

The images look the same, but a visual check cannot catch a single wrong pixel. `compare.py` loads each pair of result images and checks that their sizes and every pixel value match:

```
crop            numpy (224, 224)  c++ (224, 224)  identical: True
downsample      numpy (112, 112)  c++ (112, 112)  identical: True
flip_h          numpy (447, 447)  c++ (447, 447)  identical: True
flip_v          numpy (447, 447)  c++ (447, 447)  identical: True
negative_index  numpy (223, 223)  c++ (223, 223)  identical: True
```

All five results are identical, pixel for pixel.

## 8. Discussion

**Effort.** In NumPy, every slice is one expression, and the rules for negative indices, defaults and clamping are handled by the language. In C++, the same behavior had to be written explicitly in `indices()`. Getting the details right, especially for negative steps where the defaults and the allowed range change, is the hardest part of the task. A small mistake there would not crash the program; it would silently produce a slightly different image, which is why the pixel-by-pixel comparison is important.

**Copies versus views.** NumPy slicing creates a view: no pixels are copied, and the operation takes the same tiny amount of time no matter how large the image is. The C++ implementation copies every selected pixel into a new matrix, so its cost grows with the size of the result. A view-based design is possible in C++ too, by storing a pointer to the original data together with a start offset and a step for each dimension, but it requires much more code.

**Data structure.** The C++ version stores the matrix as `std::vector<std::vector<int>>`, a vector of rows. This is simple to read and index (`m[r][c]`), but each row is a separate allocation. A single flat array, as NumPy uses, would be faster and more memory-efficient for large images.

**Image input and output.** Python reads and writes images with Pillow in one line. C++ has no built-in image support, so the external stb library is used only for reading and writing PNG files. All slicing is done by the code in `slicing.cpp`.

**Limitations.** A step of 0 is not handled in the C++ version; Python raises an error in that case. The implementation also only supports `int` matrices, while NumPy works with any element type.

## 9. Conclusion

2D slicing was implemented with NumPy's built-in syntax and by hand in C++, following Python's rules for start, stop, step, negative indices and clamping. Both implementations give identical results on a small test matrix and on a 447 × 447 grayscale photo, as shown by the side-by-side images and confirmed by a pixel-by-pixel comparison. NumPy makes slicing a one-line operation that does not even copy data, while the C++ version shows how much logic that single line hides.

## 10. Comparison with ChatGPT's Solution

After finishing my own implementation, I asked ChatGPT to solve the same task. Its code is stored in `code/chatgpt/`. The Python code was received with its line breaks lost when copying, so they were restored; nothing else was changed.

### 10.1 How ChatGPT solved the task

ChatGPT chose **C** and wrote two short programs:

- `slicing.py` creates a 5×5 matrix with the values 1–25 and prints `matrix[1:4, 1:4]`.
- `slicing.c` stores the same matrix in a fixed-size array and prints the same region with two nested loops from `row_start` to `row_end` and from `col_start` to `col_end`.

For the graphical part, it described a text layout with both outputs side by side and offered to generate a PNG of it, but did not apply slicing to an actual image.

I ran both programs from `code/chatgpt/`. Both print the same 3×3 result, `7 8 9 / 12 13 14 / 17 18 19`:

```powershell
python slicing.py

gcc -std=c99 -Wall slicing.c -o slicing.exe
.\slicing.exe
```

### 10.2 What ChatGPT did well

- **Very simple and easy to follow.** The C version shows the core idea of slicing in a few lines: two nested loops over a start/end range are equivalent to `matrix[1:4, 1:4]`.
- **Correct for the case it covers.** Both programs give the same result for the single slice it tested.

### 10.3 Weaknesses

- **Only basic crops are supported.** There is no step, no negative indices and no default start or stop, so slices like `m[::2, ::2]`, `m[:, ::-1]` or `m[-2:, -3:]` cannot be expressed. My `indices()` function implements all of Python's slicing rules and was tested on all of these cases.
- **No real result to compare.** The C code prints values inside the loop instead of building a new matrix, and there is no reusable function. The result therefore cannot be saved as an image or checked automatically, and the "graphical image" was a hand-written text layout rather than a picture generated from both programs' actual output. My solution slices a real photo in both languages and verifies every pixel with `compare.py`.

### 10.4 Summary

| | My solution | ChatGPT's solution |
|---|---|---|
| Language | C++ | C |
| Slicing rules | start, stop, step, negative indices, defaults | start and stop only |
| Reusable slicing function | Yes (`slice2d`, `indices`) | No (loops inside `main`) |
| Result stored as a new matrix | Yes | No, printed directly |
| Applied to a real image | Yes, 447 × 447 photo, 5 slices | No, text layout only |
| Verification | Pixel-by-pixel with `compare.py` | Visual comparison of printed output |

