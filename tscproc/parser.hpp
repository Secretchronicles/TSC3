#ifndef TSCPROC_PARSER_HPP
#define TSCPROC_PARSER_HPP
#include <string>
#include <vector>

/* Bounding box. If a tile in a tileset has
 * no bounding box, the `x' and `y' members are set to -1. */
struct bbox {
    int x;
    int y;
    int w;
    int h;
};


std::vector<bbox> parse_tileset_metadata(const std::string& path);

#endif /* TSCPROC_PARSER_HPP */
