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

#include <vector>
#include <iostream>
#include <fstream>
#include <tuple>
#include <cstdio>
#include <cstdlib>
#include <csetjmp>
#include <png.h>
#include "commandline.hpp"
#include "parser.hpp"

using namespace std;

/* How many bytes libpng should check for whether we're dealing
 * with a PNG file. */
static const size_t PNG_MAGIC_BYTE_COUNT = 8;

bool check_if_png(FILE* fp)
{
    unsigned char buf[PNG_MAGIC_BYTE_COUNT];

    if (fread(buf, 1, PNG_MAGIC_BYTE_COUNT, fp) != PNG_MAGIC_BYTE_COUNT)
        return false;

    return !png_sig_cmp(buf, 0, PNG_MAGIC_BYTE_COUNT);
}

/* Checks whether a pixel is painted. The assumptions is that anything
 * where the user drew the bounding box is not fully transparent, i.e.
 * only alpha is checked. Checking the RGB values would be difficult,
 * because different image programmes can set different values for
 * fully-transparent pixels. The GIMP sets RGBA=(0|0|0|0), i.e.
 * fully transparent black, but other programmes may as well just
 * do RGBA=(255|255|255|0) or however they see fit. Thus, the alpha
 * value is the only one one can realy on. */
bool is_painted_pixel(png_byte* pix)
{
    /* Third component is alpha value;
     * 0=transparent, 255=opaque as per PNG format spec. */
    return pix[3] > 0;
}

vector<bbox> read_bboxes(int imgwidth, int imgheight, png_bytep row_pointers[])
{
    int tilewidth  = imgwidth  / cmdline.htiles;
    int tileheight = imgheight / cmdline.vtiles;

    vector<bbox> bboxes(cmdline.htiles * cmdline.vtiles);

    // Tile by tile, determine the bboxes.
    for(int tileno=0; tileno < cmdline.htiles * cmdline.vtiles; tileno++) {
        int vstart = tileno / cmdline.htiles * tileheight;
        int vend   = vstart + tileheight; // excluded, this is one behind the last row

        // Get element to work with
        struct bbox& box = bboxes[tileno];
        box.x = box.y = box.w = box.h = -1;

        for(int y=vstart; y < vend; y++) {
            png_bytep row = row_pointers[y];

            if (box.x >= 0 && box.w >= 0) {
                /* 3. Determine the height of the box. The bottom-left corner
                 * is used for that, and that one's reached either when the loop
                 * enters white pixels again or the next row is beyond the tile's
                 * edge. */
                png_bytep p_pixel = &(row[box.x*4]);

                if (!is_painted_pixel(p_pixel) || y+1 >= vend) {
                    box.h = y - box.y + 1;
                    break; // Skip rest of tile iteration, tile is complete.
                }

                continue; // Skip row iteration, not needed anymore
            }

            int hstart = tileno % cmdline.htiles * tilewidth; // included, this is the first column
            int hend   = hstart + tilewidth; // excluded, this is one behind the last column
            for(int x=hstart; x < hend; x++) {
                png_bytep p_pixel = &(row[x*4]);

                if (box.x < 0) {
                    // 1. Determine the top-left corner of the bbox.
                    if (is_painted_pixel(p_pixel)) {
                        box.x = x;
                        box.y = y;
                    }
                }
                else if (box.w < 0) {
                    /* 2. Determine the width of the bbox. The top-right corner
                     * is used for that, and that one's reached either when the
                     * loop enters white pixels again or the next column is beyond
                     * the tile's edge. */
                    if (!is_painted_pixel(p_pixel) || x+1 >= hend) {
                        box.w = x - box.x + 1;
                    }
                }
            }
        }
    }

    return bboxes;
}

vector<bbox> extract_bboxes(FILE* infile)
{
    png_structp p_png    = NULL;
    png_infop p_png_info = NULL;
    vector<bbox> bboxes;

    p_png = png_create_read_struct(PNG_LIBPNG_VER_STRING,
                                   NULL, NULL, NULL);
    if (!p_png) {
        cerr << "Internal error: cannot create PNG struct" << endl;
        return bboxes;
    }

    if (!(p_png_info = png_create_info_struct(p_png))) {
        png_destroy_read_struct(&p_png, NULL, NULL);
        cerr << "Internal error: cannot create PNG info struct" << endl;
        return bboxes;
    }

    /* If libpng wants to signal an error, it will longjmp() here
     * (see libpng manual). */
    if (setjmp(png_jmpbuf(p_png))) {
        png_destroy_read_struct(&p_png, &p_png_info, NULL);
        cerr << "Internal error while reading the PNG file" << endl;
        return bboxes;
    }

    // Read PNG header
    png_init_io(p_png, infile);
    png_set_sig_bytes(p_png, PNG_MAGIC_BYTE_COUNT); // Check already done prior
    png_read_info(p_png, p_png_info);

    // Get some info
    int width  = png_get_image_width(p_png, p_png_info);
    int height = png_get_image_height(p_png, p_png_info);

    png_set_interlace_handling(p_png);
    png_read_update_info(p_png, p_png_info);

    // Read actual image into memory
    png_bytep* row_pointers = (png_bytep*) malloc(sizeof(png_bytep) * height);
    for (int y=0; y < height; y++)
        row_pointers[y] = (png_byte*) malloc(png_get_rowbytes(p_png, p_png_info));
    png_read_image(p_png, row_pointers);

    // Read bounding boxes
    bboxes = move(read_bboxes(width, height, row_pointers));

    // Cleanup
    png_destroy_read_struct(&p_png, &p_png_info, NULL);
    for(int y=0; y < height; y++)
        free(row_pointers[y]);
    free(row_pointers);

    return bboxes;
}

void output_xml(FILE* infile)
{
    if (cmdline.htiles <= 0 || cmdline.vtiles <= 0) {
        cerr << "Error: Rows and/or columns specification required. Did you pass -d?" << endl;
        return;
    }

    vector<bbox> bboxes = extract_bboxes(infile);

    if (bboxes.empty())
        cerr << "Warning: No collision rectangles found" << endl;

    if (cmdline.authors.empty())
        cerr << "Warning: No authors given. Pass at least one -a option." << endl;

    // Output goes to real file if passed, otherwise standard output.
    ofstream outfilefile;
    if (!cmdline.xmlfile.empty())
        outfilefile.open(cmdline.xmlfile);
    ostream& outfile = cmdline.xmlfile.empty() ? cout : outfilefile;

    outfile << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" << endl
            << "<tileset version=\"1.0\">" << endl
            << "  <cols>" << cmdline.htiles << "</cols>" << endl
            << "  <rows>" << cmdline.vtiles << "</rows>" << endl
            << "  <authors>" << endl;

    for(auto iter=cmdline.authors.begin(); iter != cmdline.authors.end(); iter++) {
        outfile << "    <author>" << endl
                << "      <name>" << iter->first << "</name>" << endl
                << "      <detail>" << iter->second << "</detail>" << endl
                << "    </author>" << endl;
    }

    outfile << "  </authors>" << endl
            << "  <tiles>" << endl;

    for(const bbox& box: bboxes) {
        outfile << "    <tile>" << endl
                << "      <colrect x=\""
                  << box.x
                  << "\" y=\""
                  << box.y
                  << "\" width=\""
                  << box.w
                  << "\" height=\""
                  << box.h
                  << "\"/>"
                  << endl
                << "    </tile>" << endl;
    }

    outfile << "  </tiles>" << endl
            << "</tileset>" << endl;
}

/**
 * Reads the size in pixels from the given PNG file and
 * returns it as a tuple (width, height). If an empty string
 * is passed, standard input is used as the file.
 */
tuple<int, int> read_dimensions(const std::string& path)
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

void input_xml()
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

int main(int argc, char* argv[])
{
    parse_commandline(argc, argv);

    if (cmdline.mode == cmdmode::metadata) {
        FILE* infile = cmdline.collfile.empty() ? stdin : fopen(cmdline.collfile.c_str(), "rb");
        if (!infile) {
            cerr << "Failed to open collision rectangle PNG file." << endl;
            return 1;
        }

        if (!check_if_png(infile)) {
            cerr << "Collision rectangle PNG is not a PNG file." << endl;
            fclose(infile);
            return 1;
        }

        output_xml(infile);
        fclose(infile);
    }
    else if (cmdline.mode == cmdmode::png) {
        input_xml();
    }
    else {
        cerr << "Unknown mode." << endl;
        return 1;
    }

    return 0;
}
