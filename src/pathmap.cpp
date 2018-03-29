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
    return Path::exe().dirname() / ".." / "share" / "tsc3";
#else
    /* INSTALL_DATADIR is assumed to be UTF-8. This breaks on systems
     * that encode the build system commandline arguments not as UTF-8.
     * Since Win32 is taken care of above already, the problem is small
     * enough to ignore; non-UTF-8 Unices have become rare. And even on
     * those, sticking to plain ASCII works in any case. */
    return Path(INSTALL_DATADIR) / "tsc3";
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

/**
 * Returns the absolute path to the directory where TSC's
 * music files are stored. The directory should be assumed
 * to be read-only.
 */
Path Pathmap::GetMusicPath() const
{
    return GetDataPath() / "music";
}

/**
 * Returns the absolute path to the directory where TSC's
 * compiled MO translations are stored.
 */
Path Pathmap::GetLocalePath() const {
    // This must match where cmake places the MO files.
    return GetDataPath() / "translations";
}

/**
 * Returns the absolute path to the directory where TSC's
 * font files are stored. This directory should be assumed
 * to be read-only.
 */
Path Pathmap::GetFontPath() const {
    return GetDataPath() / "fonts";
}
