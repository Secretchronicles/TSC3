#ifndef TSCPROC_COMMANDLINE_HPP
#define TSCPROC_COMMANDLINE_HPP
#include <string>
#include <map>

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
    std::map<std::string, std::string> authors;
};
extern cmdargs cmdline;

void parse_commandline(int argc, char* argv[]);

#endif /* TSC_COMMANDLINE_HPP */
