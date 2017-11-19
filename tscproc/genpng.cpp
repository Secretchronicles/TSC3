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

#include "genpng.hpp"
#include "commandline.hpp"
#include "util.hpp"
#include "parser.hpp"
#include <tuple>
#include <vector>
#include <iostream>
#include <string>
#include <csetjmp>
#include <cstdio>
#include <png.h>

using namespace std;

/**
 * Reads the size in pixels from the given PNG file and
 * returns it as a tuple (width, height). If an empty string
 * is passed, standard input is used as the file.
 */
tuple<int, int> read_dimensions(const string& path)
{
    png_structp p_png    = NULL;
    png_infop p_png_info = NULL;

    p_png = png_create_read_struct(PNG_LIBPNG_VER_STRING,
                                   NULL, NULL, NULL);
    if (!p_png) {
        cerr << "Internal error: cannot create PNG struct" << endl;
        exit(2);
    }

    if (!(p_png_info = png_create_info_struct(p_png))) {
        png_destroy_read_struct(&p_png, NULL, NULL);
        cerr << "Internal error: cannot create PNG info struct" << endl;
        exit(2);
    }

    /* If libpng wants to signal an error, it will longjmp() here
     * (see libpng manual). */
    if (setjmp(png_jmpbuf(p_png))) {
        png_destroy_read_struct(&p_png, &p_png_info, NULL);
        cerr << "Internal error while reading the PNG file" << endl;
        exit(2);
    }

    FILE* infile = path.empty() ? stdin : fopen(path.c_str(), "rb");
    if (!infile) {
        cerr << "Failed to open tileset file." << endl;
        exit(2);
    }

    if (!check_if_png(infile)) {
        cerr << "Tileset is not a PNG file" << endl;
        png_destroy_read_struct(&p_png, &p_png_info, NULL);
        fclose(infile);
        exit(2);
    }

    // Read PNG header
    png_init_io(p_png, infile);
    png_set_sig_bytes(p_png, PNG_MAGIC_BYTE_COUNT); // Check already done prior
    png_read_info(p_png, p_png_info);

    // Get dimension info
    tuple<int, int> dimen = make_tuple<int, int>(png_get_image_width(p_png, p_png_info),
                                                 png_get_image_height(p_png, p_png_info));

    // Cleanup
    png_destroy_read_struct(&p_png, &p_png_info, NULL);
    return dimen;
}

void generate_colrect_png()
{
    // Read collision rectangles from XML and pixel size from tileset
    vector<bbox> bboxes = parse_tileset_metadata(cmdline.xmlfile);
    tuple<int, int> size = read_dimensions(cmdline.tilesetfile);

    // Allocate the collision rectangle PNG in memory
    png_bytep* row_pointers = (png_bytep*) malloc(sizeof(png_bytep) * get<1>(size));
    for (int y=0; y < get<1>(size); y++) {
        row_pointers[y] = (png_byte*) malloc(sizeof(png_byte) * get<0>(size) * 4); // 4 bytes per pixel (RGBA)
    }

    // Now build the image contents
    for (const bbox& box: bboxes) {
        for(int y=box.y; y < box.y + box.h; y++) {
            for(int x=box.x; x < box.x + box.w; x++) {
                png_byte* p_pixel = &(row_pointers[y][x*4]);
                p_pixel[0] = 0;   // Red
                p_pixel[1] = 0;   // Green
                p_pixel[2] = 255; // Blue
                p_pixel[3] = 150; // Alpha
            }
        }
    }

    png_structp p_png = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!p_png) {
        cerr << "Failed to create PNG write struct." << endl;
        exit(2);
    }

    png_infop p_png_info = png_create_info_struct(p_png);
    if (!p_png) {
        cerr << "Failed to create PNG info struct." << endl;
        exit(2);
    }

    // Determine correct output
    FILE* outfile = NULL;
    if (cmdline.collfile.empty()) { // Write to standard output
        outfile = stdout;
    }
    else { // Write to file
        outfile = fopen(cmdline.collfile.c_str(), "wb");
        if (!outfile) {
            cerr << "Failed to open file '" << cmdline.collfile << "' for writing." << endl;
            png_destroy_write_struct(&p_png, &p_png_info);
            exit(1);
        }
    }

    /* If libpng wants to signal an error, it will longjmp() here
     * (see libpng manual). */
    if (setjmp(png_jmpbuf(p_png))) {
        cerr << "Internal error while writing PNG file." << endl;
        png_destroy_write_struct(&p_png, &p_png_info);
        fclose(outfile);
        exit(2);
    }

    // Write PNG to disk
    png_init_io(p_png, outfile);
    png_set_IHDR(p_png, p_png_info,
                 get<0>(size), get<1>(size),
                 8, PNG_COLOR_TYPE_RGB_ALPHA,
                 PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_BASE,
                 PNG_FILTER_TYPE_BASE);
    png_write_info(p_png, p_png_info);
    png_write_image(p_png, row_pointers);
    png_write_end(p_png, NULL);

    // Cleanup
    png_destroy_write_struct(&p_png, &p_png_info);
    for(int y=0; y < get<1>(size); y++)
        free(row_pointers[y]);
    free(row_pointers);
    fclose(outfile);
}
