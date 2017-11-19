/* TSC is a two-dimensional jump’n’run platform game.
 * Copyright © 2017 The TSC Contributors
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

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
