/*******************************************************************************
 * This file is part of TSC.
 *
 * TSC is a 2-dimensional platform game.
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
 ******************************************************************************/

#include "configuration.hpp"
#include "util.hpp"
#include "xerces_helpers.hpp"
#include <xercesc/sax2/DefaultHandler.hpp>
#include <xercesc/sax2/SAX2XMLReader.hpp>
#include <xercesc/sax2/Attributes.hpp>
#include <xercesc/sax2/XMLReaderFactory.hpp>
#include <xercesc/framework/LocalFileInputSource.hpp>

using namespace TSC;
using namespace xercesc;
using namespace std;

namespace {
    class ConfigurationHandler: public DefaultHandler {
    public:
        ConfigurationHandler(Configuration& config)
            : mr_config(config)
        {
        }

        virtual void startElement(const XMLCh* const,
                                  const XMLCh* const xlocalname,
                                  const XMLCh* const,
                                  const Attributes& attrs)
            {
                m_chars.clear();
            }

        virtual void endElement(const XMLCh* const,
                                const XMLCh* const xlocalname,
                                const XMLCh* const)
            {
                string localname = xstr_to_utf8(xlocalname);
                if (localname == "game_version")
                    mr_config.game_version = m_chars;
                else if (localname == "screen_width")
                    mr_config.screen_width = stoi(m_chars);
                else if (localname == "screen_height")
                    mr_config.screen_height = stoi(m_chars);
                else if (localname == "screen_bpp")
                    mr_config.screen_bpp = stoi(m_chars);
                else if (localname == "enable_vsync")
                    mr_config.enable_vsync = m_chars == "yes";
                else if (localname == "enable_always_run")
                    mr_config.enable_always_run = m_chars == "yes";
                else if (localname == "enable_fullscreen")
                    mr_config.enable_fullscreen = m_chars == "yes";
                else if (localname == "enable_music")
                    mr_config.enable_music = m_chars == "yes";
                else if (localname == "enable_sound")
                    mr_config.enable_sound = m_chars == "yes";
                else if (localname == "music_volume") {
                    mr_config.music_volume = stoi(m_chars);
                    if (mr_config.music_volume < 0)
                        mr_config.music_volume = 0;
                    else if (mr_config.music_volume > 100)
                        mr_config.music_volume = 100;
                }
                else if (localname == "sound_volume") {
                    mr_config.sound_volume = stoi(m_chars);
                    if (mr_config.sound_volume < 0)
                        mr_config.sound_volume = 0;
                    else if (mr_config.sound_volume > 100)
                        mr_config.sound_volume = 100;
                }
                else {
                    warn(string("Ignoring unknown configuration parameter: ") + localname);
                }
            }

        virtual void characters(const XMLCh* const chars, const XMLSize_t)
        {
            m_chars.append(xstr_to_utf8(chars));
        }

    private:
        Configuration& mr_config;
        string m_chars;
    };
}

/**
 * Constructs a new Configuration instance that reads the configuration from
 * the passed file. The file is immediately parsed in the constructor, which
 * hence might throw exceptions just as Xerces-C does.
 */
Configuration::Configuration(string path)
    : m_path(path)
{
    SAX2XMLReader* p_reader = XMLReaderFactory::createXMLReader();
    p_reader->setFeature(XMLUni::fgSAX2CoreValidation, false);

    ConfigurationHandler handler(*this);
    p_reader->setContentHandler(&handler);
    p_reader->setErrorHandler(&handler);

    LocalFileInputSource source(utf8_to_xstr(path).get());
    p_reader->parse(source);
}

Configuration::~Configuration()
{
}
