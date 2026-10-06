#ifndef SLICING_H
#define SLICING_H

#include <optional>
#include <vector>

using Matrix = std::vector<std::vector<int>>;

//start:stop:step in Python, an empty start or stop means None.
struct Slice {
    std::optional<long> start, stop;
    long step = 1;
};
std::vector<long> indices(Slice s, long n);
//rturns m[rows, cols], like 2D slicing in NumPy.
Matrix slice2d(const Matrix &m, Slice rows, Slice cols);

#endif
