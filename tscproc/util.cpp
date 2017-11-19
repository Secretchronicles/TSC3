#include "util.hpp"

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
