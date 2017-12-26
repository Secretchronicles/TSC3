#include "pathmap.hpp"
#include "config.hpp"

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

/**
 * Returns the absolute path to the directory where TSC's
 * assets are stored. This directoy should be assumed to
 * be read-only as it will usually reside below /usr.
 */
Path Pathmap::GetDataPath() const
{
#ifdef _WIN32
    return Path::exe().dirname() / ".." / "share" / "tsc";
#else
    /* INSTALL_DATADIR is assumed to be UTF-8. This breaks on systems
     * that encode the build system commandline arguments not as UTF-8.
     * Since Win32 is taken care of above already, the problem is small
     * enough to ignore; non-UTF-8 Unices have become rare. And even on
     * those, sticking to plain ASCII works in any case. */
    return Path(INSTALL_DATADIR);
#endif
}

/**
 * Returns the absolute path to the directly where TSC's
 * pixmaps are stored. The directory should be assumed to
 * be read-only.
 */
Path Pathmap::GetPixmapsPath() const
{
    return GetDataPath() / "pixmaps";
}
