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
"MODES:\n"
"\n"
"  -M           Output a metadata XML file.\n"
"  -P           Output a bbox PNG file.\n"
"\n"
"OPTIONS:\n"
"\n"
"  -a NAME:DESC  Add one <author> info with author name and\n"
"                detailed description. Only used if input is\n"
"                a PNG file. Can be passed multiple times. (only -M)\n"
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
            case 'a':
                if (i + 1 >= argc)
                    print_help();
                else {
                    string authorstr = argv[++i];
                    int delim = authorstr.find(":");
                    cmdline.authors[authorstr.substr(0, delim)] = authorstr.substr(delim+1);
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
