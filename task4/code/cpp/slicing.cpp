#include "slicing.h"

#include <algorithm>

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

Matrix slice2d(const Matrix &m, Slice rows, Slice cols)
{
    long n_rows = m.size();
    long n_cols = m.empty() ? 0 : m[0].size();

    Matrix result;
    for (long r : indices(rows, n_rows)) {
        std::vector<int> row;
        for (long c : indices(cols, n_cols)) {
            row.push_back(m[r][c]);
        }
        result.push_back(row);
    }
    return result;
}
