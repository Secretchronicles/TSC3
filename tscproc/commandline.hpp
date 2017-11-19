#ifndef TSCPROC_COMMANDLINE_HPP
#define TSCPROC_COMMANDLINE_HPP
#include <string>
#include <map>

struct cmdargs {
    bool input_is_png;
    int htiles;
    int vtiles;
    std::string infile;
    std::string outfile;
    std::map<std::string, std::string> authors;
};
extern cmdargs cmdline;

void parse_commandline(int argc, char* argv[]);

#endif /* TSC_COMMANDLINE_HPP */
