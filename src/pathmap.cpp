/*******************************************************************************
 * This file is part of TSC.
 *
 * TSC is a 2-dimensional platform game.
 * Copyright © 2018 The TSC Contributors
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
 ******************************************************************************/

#include "pathmap.hpp"
#include "config.hpp"

// Short name for the directory to use below ~/.local/share, ~/.config, etc.
#define TSCDIR "tsc3"

using namespace TSC;
using namespace Pathie;

/**
 * Returns the absolute path to TSC's configuration file.
 */
Path Pathmap::GetConfigPath()
{
    return Path::config_dir() / TSCDIR / "config.xml";
}

/**
 * Returns the absolute path to the directory where TSC's
 * assets are stored. This directoy should be assumed to
 * be read-only as it will usually reside below /usr.
 */
Path Pathmap::GetDataPath()
{
#ifdef _WIN32
    return Path::exe().dirname() / ".." / "share" / TSCDIR;
#else
    /* INSTALL_DATADIR is assumed to be UTF-8. This breaks on systems
     * that encode the build system commandline arguments not as UTF-8.
     * Since Win32 is taken care of above already, the problem is small
     * enough to ignore; non-UTF-8 Unices have become rare. And even on
     * those, sticking to plain ASCII works in any case. */
    return Path(INSTALL_DATADIR) / TSCDIR;
#endif
}

Path Pathmap::GetUserDataPath()
{
    return Path::data_dir() / TSCDIR;
}

/**
 * Returns the absolute path to the directory where TSC's
 * pixmaps are stored. The directory should be assumed to
 * be read-only.
 */
Path Pathmap::GetPixmapsPath()
{
    return GetDataPath() / "pixmaps";
}

/**
 * Returns the absolute path to the directory where TSC's
 * built-in levels are stored. This directory should be assumed
 * to be read-only.
 */
Path Pathmap::GetGlobalLevelsPath()
{
    return GetDataPath() / "levels";
}

/**
 * Returns the absolute path to the requested global
 * level.
 */
Path Pathmap::GetGlobalLevelPath(const std::string& relfilename)
{
    return GetGlobalLevelsPath() / relfilename;
}

/**
 * Returns the absolute path to the directory where
 * the user can store his levels.
 */
Path Pathmap::GetUserLevelsPath()
{
    return GetUserDataPath() / "levels";
}

/**
 * Returns the absolute path to the requested user level.
 */
Path Pathmap::GetUserLevelPath(const std::string& relfilename)
{
    return GetUserLevelsPath() / relfilename;
}

/**
 * Retrieves the absolute path for the given level.
 * This function first tries to find the level in the
 * user level directory (see GetUserLevelsPath()). If
 * the level doesn't exist there, it tries the global
 * level directory (see GetGlobalLevelsPath()). If the
 * file is not found there either, throws.
 */
Path Pathmap::GetLevelPath(const std::string& relfilename)
{
    Path user_level = GetUserLevelPath(relfilename);
    if (user_level.exists())
        return user_level;

    Path global_level = GetGlobalLevelPath(relfilename);
    if (global_level.exists())
        return global_level;

    throw(std::runtime_error(std::string("Requested level file not found: ") + relfilename));
}

/**
 * Returns the absolute path to the directory where TSC's
 * music files are stored. The directory should be assumed
 * to be read-only.
 */
Path Pathmap::GetMusicPath()
{
    return GetDataPath() / "music";
}

/**
 * Returns the absolute path to the directory where TSC's
 * compiled MO translations are stored.
 */
Path Pathmap::GetLocalePath() {
    // This must match where cmake places the MO files.
    return GetDataPath() / "translations";
}

/**
 * Returns the absolute path to the directory where TSC's
 * font files are stored. This directory should be assumed
 * to be read-only.
 */
Path Pathmap::GetFontPath() {
    return GetDataPath() / "fonts";
}
