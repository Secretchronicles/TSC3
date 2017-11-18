#ifndef TSCPROC_PARSER_HPP
#define TSCPROC_PARSER_HPP
#include <string>
#include <tuple>
#include <vector>

std::vector<std::tuple<int, int, int, int>> parse_tileset(const std::string& path);

#endif /* TSCPROC_PARSER_HPP */
