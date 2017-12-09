#include "pathmap.hpp"

using namespace TSC;
using namespace Pathie;

Pathmap::Pathmap()
{
    //
}

/**
 * Returns the absolute path to TSC's configuration file.
 */
Path Pathmap::GetConfigPath() const
{
    return Path::config_dir() / "tsc3" / "config.xml";
}
