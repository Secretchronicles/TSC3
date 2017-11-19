#include "genmeta.hpp"
#include "commandline.hpp"
#include "util.hpp"
#include <iostream>
#include <fstream>
#include <vector>
#include <png.h>

using namespace std;

static vector<bbox> read_bboxes(int imgwidth, int imgheight, png_bytep row_pointers[])
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

static vector<bbox> extract_bboxes(FILE* infile)
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

static void output_xml(FILE* infile)
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

void generate_meta_xml()
{
    FILE* infile = cmdline.collfile.empty() ? stdin : fopen(cmdline.collfile.c_str(), "rb");
    if (!infile) {
        cerr << "Failed to open collision rectangle PNG file." << endl;
        exit(1);
    }

    if (!check_if_png(infile)) {
        cerr << "Collision rectangle PNG is not a PNG file." << endl;
        fclose(infile);
        exit(1);
    }

    output_xml(infile);
    fclose(infile);
}
