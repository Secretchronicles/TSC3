#ifndef TSCPROC_UTIL_HPP
#define TSCPROC_UTIL_HPP
#include <cstdio>
#include <png.h>

/* How many bytes libpng should check for whether we're dealing
 * with a PNG file. */
#define PNG_MAGIC_BYTE_COUNT 8

/* Bounding box. If a tile in a tileset has
 * no bounding box, the `x' and `y' members are set to -1. */
struct bbox {
    int x;
    int y;
    int w;
    int h;
};

bool check_if_png(FILE* fp);
bool is_painted_pixel(png_byte* pix);

#endif /* TSCPROC_UTIL_HPP */
