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

#ifndef TSCPROC_COMMANDLINE_HPP
#define TSCPROC_COMMANDLINE_HPP
#include <string>

enum class cmdmode {
    none = 0,
    png,
    metadata
};

struct cmdargs {
    int htiles;
    int vtiles;
    std::string tilesetfile;
    std::string collfile;
    std::string xmlfile;
    cmdmode mode;
};
extern cmdargs cmdline;

void parse_commandline(int argc, char* argv[]);

#endif /* TSC_COMMANDLINE_HPP */
