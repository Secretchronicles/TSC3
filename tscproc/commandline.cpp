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

#include "commandline.hpp"
#include <string>
#include <iostream>

using namespace std;

// extern declarations
cmdargs cmdline;

static void print_help()
{
    cout << "USAGE: tscproc [MODE] [OPTIONS]\n"
"\n"
"tscproc processes TSC's tile metadata files. It operates on one\n"
"of the modes described below. Depending on the mode, different\n"
"options can be passed to this programme.\n"
"\n"
"The bounding box (bbox) PNG file describes the actual tileset's\n"
"bounding boxes. Everywhere you want a bounding box on the tileset,\n"
"you should color the bbox image, which should be of the exact same\n"
"dimensions as the actual tileset image."
"\n"
"MODES:\n"
"\n"
"  -M           Output a metadata XML file.\n"
"  -P           Output a bbox PNG file.\n"
"\n"
"OPTIONS:\n"
"\n"
"  -c FILE       Collision box PNG file.\n"
"  -d ROWS:COLS  If the input is a PNG file, this option gives\n"
"                the number of rows and columns the tileset has,\n"
"                in numbers of tiles. (only -M)\n"
"  -h            Print this help.\n"
"  -t FILE       Tileset PNG file. Pass - for standard input. (only -P)\n"
"  -x FILE       Metadata XML file. Pass - for standard output.\n";
    exit(3);
}

/**
 * Parsers the given commandline arguments and thereby populates
 * the `cmdline` global variable.
 *
 * Exits the programme with an error message on commandline
 * usage error.
 */
void parse_commandline(int argc, char* argv[])
{
    cmdline.htiles = 0;
    cmdline.vtiles = 0;
    cmdline.mode   = cmdmode::none;

    for(int i=1; i < argc; i++) {
        string arg = string(argv[i]);

        if (arg[0] == '-') { // Option
            switch (arg[1]) {
            case 'd':
                if (i + 1 >= argc)
                    print_help();
                else {
                    string tilestr = argv[++i];
                    int delim      = tilestr.find(":");
                    cmdline.vtiles = stoi(tilestr.substr(0, delim));
                    cmdline.htiles = stoi(tilestr.substr(delim+1));
                }
                break;
            case 't':
                if (i + 1 >= argc)
                    print_help();

                cmdline.tilesetfile = argv[++i];

                if (cmdline.tilesetfile == "-")
                    cmdline.tilesetfile.clear();

                break;
            case 'x':
                if (i + 1 >= argc)
                    print_help();

                cmdline.xmlfile = argv[++i];

                if (cmdline.xmlfile == "-")
                    cmdline.xmlfile.clear();

                break;
            case 'c':
                if (i + 1 >= argc)
                    print_help();

                cmdline.collfile = argv[++i];

                if (cmdline.collfile == "-")
                    cmdline.collfile.clear();

                break;
            case 'P':
                cmdline.mode = cmdmode::png;
                break;
            case 'M':
                cmdline.mode = cmdmode::metadata;
                break;
            case 'h':
                print_help();
                break;
            default:
                cerr << "Unknown argument " << arg << endl;
                print_help();
                break;
            }
        }
    }

    if (cmdline.mode == cmdmode::none) {
        cerr << "No mode specified." << endl;
        print_help();
    }
}
