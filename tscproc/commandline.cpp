#include "commandline.hpp"
#include <string>
#include <iostream>

using namespace std;

// extern declarations
cmdargs cmdline;

static void print_help()
{
    cout << "USAGE: tscproc [OPTIONS]\n"
"\n"
"tscproc processes TSC's tile metadata files. If given a PNG\n"
"file as input, it determines the collision rectangle for each\n"
"tile from it by searching for areas not fully transparent. If\n"
"given an XML file as input, it generates a PNG file describing\n"
"the collision rectangles.\n"
"\n"
"If read from standard input is requested, one of the options -P\n"
"(assume PNG input) or -X (assume XML input) is required. Otherwise\n"
"the operational mode is determined by looking at the file extension\n"
"of the file given as input.\n"
"\n"
"OPTIONS:\n"
"\n"
"  -a NAME:DESC  Add one <author> info with author name and\n"
"                detailed description. Only used if input is\n"
"                a PNG file. Can be passed multiple times.\n"
"  -h            Print this help.\n"
"  -i INPUT      Input file. Pass - for standard input.\n"
"  -o OUTPUT     Output file. Pass - for standard output.\n"
"  -P            Force input to be treated as a PNG file.\n"
"  -t ROWS:COLS  If the input is a PNG file, this option gives\n"
"                the number of rows and columns the tileset has,\n"
"                in numbers of tiles.\n"
" -X             Force input to be treated as an XML file.\n";
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
    bool mode_known = false;

    for(int i=1; i < argc; i++) {
        string arg = string(argv[i]);

        if (arg[0] == '-') { // Option
            switch (arg[1]) {
            case 't':
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
            case 'i':
                if (i + 1 >= argc)
                    print_help();

                cmdline.infile = argv[++i];

                // stdin is used if infile is empty
                if (cmdline.infile == "-")
                    cmdline.infile.clear();

                break;
            case 'o':
                if (i + 1 >= argc)
                    print_help();

                cmdline.outfile = argv[++i];

                // stdout is used if outfile is empty
                if (cmdline.outfile == "-")
                    cmdline.outfile.clear();

                break;
            case 'P':
                cmdline.input_is_png = true;
                mode_known = true;
                break;
            case 'X':
                cmdline.input_is_png = false;
                mode_known = true;
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

    if (mode_known)
        return;
    if (cmdline.infile.empty()) {
        cerr << "If standard input is used as input, you have to pass either the -P or the -X option." << endl;
        print_help();
    }

    cmdline.input_is_png = cmdline.infile.find(".png") != string::npos;
}
