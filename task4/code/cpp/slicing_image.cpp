#include <cstdio>
#include <string>

#include "slicing.h"
#include "stb_image.h"
#include "stb_image_write.h"

// Reads a grayscale PNG into a Matrix (one value 0-255 per pixel).
Matrix load_png(const char *path)
{
    int w, h, channels;
    unsigned char *pixels = stbi_load(path, &w, &h, &channels, 1);
    if (!pixels) return {};

    Matrix m(h, std::vector<int>(w));
    for (int r = 0; r < h; r++)
        for (int c = 0; c < w; c++)
            m[r][c] = pixels[r * w + c];

    stbi_image_free(pixels);
    return m;
}

void save_png(const std::string &path, const Matrix &m)
{
    int h = m.size();
    int w = m.empty() ? 0 : m[0].size();

    std::vector<unsigned char> pixels;
    for (const auto &row : m)
        for (int v : row)
            pixels.push_back(static_cast<unsigned char>(v));

    stbi_write_png(path.c_str(), w, h, 1, pixels.data(), w);
}

int main()
{
    Matrix img = load_png("../python/images/input.png");
    if (img.empty()) {
        printf("Could not read ../python/images/input.png. Run slicing_image.py first.\n");
        return 1;
    }
    long h = img.size(), w = img[0].size();
    auto none = std::nullopt;

    // Same cases as slicing_image.py.
    struct Case {
        const char *name;
        Slice rows, cols;
    };
    Case cases[] = {
        {"crop",           {h / 4, 3 * h / 4, 1},  {w / 4, 3 * w / 4, 1}},
        {"downsample",     {none, none, 4},        {none, none, 4}},
        {"flip_h",         {none, none, 1},        {none, none, -1}},
        {"flip_v",         {none, none, -1},       {none, none, 1}},
        {"negative_index", {-(h / 2), none, 1},    {-(w / 2), none, 1}},
    };

    printf("input: %ld x %ld\n", h, w);
    for (const Case &c : cases) {
        Matrix result = slice2d(img, c.rows, c.cols);
        save_png(std::string("images/cpp_") + c.name + ".png", result);
        printf("%s: %zu x %zu\n", c.name, result.size(), result[0].size());
    }
}
