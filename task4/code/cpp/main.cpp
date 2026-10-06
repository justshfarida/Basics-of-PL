#include <cstdio>

#include "slicing.h"

void show(const char *title, const Matrix &m)
{
    printf("\n%s -> %zu x %zu\n", title, m.size(), m.empty() ? 0 : m[0].size());
    for (const auto &row : m) {
        for (int v : row) printf("%4d", v);
        printf("\n");
    }
}

int main()
{
    // Each value is 10 * row + column, so 23 means row 2, column 3.
    Matrix m(5, std::vector<int>(6));
    for (int r = 0; r < 5; r++)
        for (int c = 0; c < 6; c++)
            m[r][c] = 10 * r + c;

    auto none = std::nullopt;

    show("original", m);
    show("m[1:4, 2:5]",     slice2d(m, {1, 4, 1},        {2, 5, 1}));
    show("m[::2, ::2]",     slice2d(m, {none, none, 2},  {none, none, 2}));
    show("m[:, ::-1]",      slice2d(m, {none, none, 1},  {none, none, -1}));
    show("m[::-1, :]",      slice2d(m, {none, none, -1}, {none, none, 1}));
    show("m[-2:, -3:]",     slice2d(m, {-2, none, 1},    {-3, none, 1}));
}
