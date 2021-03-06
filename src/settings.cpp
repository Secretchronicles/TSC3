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

#include "settings.hpp"
#include "config.hpp"
#include "util.hpp"
#include "xerces_helpers.hpp"
#include "pathmap.hpp"
#include "application.hpp"
#include <pathie/path.hpp>
#include <xercesc/sax2/DefaultHandler.hpp>
#include <xercesc/sax2/SAX2XMLReader.hpp>
#include <xercesc/sax2/Attributes.hpp>
#include <xercesc/sax2/XMLReaderFactory.hpp>
#include <xercesc/framework/LocalFileInputSource.hpp>
#include <xercesc/framework/LocalFileFormatTarget.hpp>
#include <xercesc/dom/DOMImplementation.hpp>
#include <xercesc/dom/DOMImplementationRegistry.hpp>
#include <xercesc/dom/DOMElement.hpp>
#include <xercesc/dom/DOMText.hpp>
#include <xercesc/dom/DOMDocument.hpp>
#include <xercesc/dom/DOMLSSerializer.hpp>
#include <xercesc/dom/DOMLSOutput.hpp>
#include <xercesc/dom/DOMConfiguration.hpp>

using namespace TSC;
using namespace xercesc;
using namespace Pathie;
using namespace std;

// The default settings values
int Settings::screen_width       = NATIVE_WIDTH;
int Settings::screen_height      = NATIVE_HEIGHT;
int Settings::music_volume       = 100;
int Settings::sound_volume       = 100;

bool Settings::enable_vsync      = false;
bool Settings::enable_always_run = false;
bool Settings::enable_fullscreen = true;
bool Settings::enable_music      = true;
bool Settings::enable_sound      = true;

// This does not have a default value. It is required to be present
// in the configuration file.
std::string Settings::game_version;

namespace {
    class ConfigurationHandler: public DefaultHandler {
    public:
        virtual void startElement(const XMLCh* const,
                                  const XMLCh* const,
                                  const XMLCh* const,
                                  const Attributes&)
        {
            m_chars.clear();
        }

        virtual void endElement(const XMLCh* const,
                                const XMLCh* const xlocalname,
                                const XMLCh* const)
        {
            string localname = X2U(xlocalname);
            if (localname == "game_version")
                Settings::game_version = m_chars;
            else if (localname == "screen_width")
                Settings::screen_width = stoi(m_chars);
            else if (localname == "screen_height")
                Settings::screen_height = stoi(m_chars);
            else if (localname == "enable_vsync")
                Settings::enable_vsync = m_chars == "yes";
            else if (localname == "enable_always_run")
                Settings::enable_always_run = m_chars == "yes";
            else if (localname == "enable_fullscreen")
                Settings::enable_fullscreen = m_chars == "yes";
            else if (localname == "enable_music")
                Settings::enable_music = m_chars == "yes";
            else if (localname == "enable_sound")
                Settings::enable_sound = m_chars == "yes";
            else if (localname == "music_volume") {
                Settings::music_volume = stoi(m_chars);
                if (Settings::music_volume < 0)
                    Settings::music_volume = 0;
                else if (Settings::music_volume > 100)
                    Settings::music_volume = 100;
            }
            else if (localname == "sound_volume") {
                Settings::sound_volume = stoi(m_chars);
                if (Settings::sound_volume < 0)
                    Settings::sound_volume = 0;
                else if (Settings::sound_volume > 100)
                    Settings::sound_volume = 100;
            }
            else if (localname == "configuration") {
                // Ignore root node
            }
            else {
                warn(string("Ignoring unknown configuration parameter: ") + localname);
            }
        }

        virtual void characters(const XMLCh* const chars, const XMLSize_t)
        {
            m_chars.append(X2U(chars));
        }

    private:
        string m_chars;
    };
}

/**
 * Read the configuration file and adapt the global settings
 * accordingly. This method can throw the same exceptions as
 * Xerces-C does when parsing a file.
 */
void Settings::Load()
{
    if (Pathmap::GetConfigPath().exists()) {
        SAX2XMLReader* p_reader = XMLReaderFactory::createXMLReader();
        p_reader->setFeature(XMLUni::fgSAX2CoreValidation, false);

        ConfigurationHandler handler;
        p_reader->setContentHandler(&handler);
        p_reader->setErrorHandler(&handler);

        LocalFileInputSource source(U2X(Pathmap::GetConfigPath().utf8_str()));
        p_reader->parse(source);

        // A version compatibility check would go here.

        // Update version
        game_version = to_string(TSC_VERSION_MAJOR) +
                       "." +
                       to_string(TSC_VERSION_MINOR) +
                       "." +
                       to_string(TSC_VERSION_PATCH);
    }
    else {
        // Configuration file does not exist, save default config to it.
        game_version = to_string(TSC_VERSION_MAJOR) +
                       "." +
                       to_string(TSC_VERSION_MINOR) +
                       "." +
                       to_string(TSC_VERSION_PATCH);

        Settings::Save();
    }
}

/**
 * Save the configuration in its current state into the configuration
 * file that was specified in the constructor. If the directory
 * for the configuration file does not exist, it is created.
 */
void Settings::Save()
{
    Pathmap::GetConfigPath().parent().mktree();

    // LS = Load+Save
    DOMImplementation* p_impl = DOMImplementationRegistry::getDOMImplementation(
                                    U2X("Core LS"));
    if (!p_impl) {
        warn("Xerces-C error on saving the configuration: no DOM Implemenation with Core+LS found");
        return;
    }

    // Create the DOM tree
    DOMDocument* p_doc = p_impl->createDocument(nullptr, U2X("configuration"), NULL);
    DOMElement* p_root = p_doc->getDocumentElement();

    p_doc->setXmlStandalone(true);

    DOMElement* p_child = p_doc->createElement(U2X("game_version"));
    DOMText* p_text = p_doc->createTextNode(U2X(game_version));
    p_child->appendChild(p_text);
    p_root->appendChild(p_child);

    p_child = p_doc->createElement(U2X("screen_width"));
    p_text = p_doc->createTextNode(U2X(to_string(screen_width)));
    p_child->appendChild(p_text);
    p_root->appendChild(p_child);

    p_child = p_doc->createElement(U2X("screen_height"));
    p_text = p_doc->createTextNode(U2X(to_string(screen_height)));
    p_child->appendChild(p_text);
    p_root->appendChild(p_child);

    p_child = p_doc->createElement(U2X("music_volume"));
    p_text = p_doc->createTextNode(U2X(to_string(music_volume)));
    p_child->appendChild(p_text);
    p_root->appendChild(p_child);

    p_child = p_doc->createElement(U2X("sound_volume"));
    p_text = p_doc->createTextNode(U2X(to_string(sound_volume)));
    p_child->appendChild(p_text);
    p_root->appendChild(p_child);

    p_child = p_doc->createElement(U2X("enable_vsync"));
    p_text = p_doc->createTextNode(U2X(enable_vsync ? "yes" : "no"));
    p_child->appendChild(p_text);
    p_root->appendChild(p_child);

    p_child = p_doc->createElement(U2X("enable_always_run"));
    p_text = p_doc->createTextNode(U2X(enable_always_run ? "yes" : "no"));
    p_child->appendChild(p_text);
    p_root->appendChild(p_child);

    p_child = p_doc->createElement(U2X("enable_fullscreen"));
    p_text = p_doc->createTextNode(U2X(enable_fullscreen ? "yes" : "no"));
    p_child->appendChild(p_text);
    p_root->appendChild(p_child);

    p_child = p_doc->createElement(U2X("enable_music"));
    p_text = p_doc->createTextNode(U2X(enable_music ? "yes" : "no"));
    p_child->appendChild(p_text);
    p_root->appendChild(p_child);

    p_child = p_doc->createElement(U2X("enable_sound"));
    p_text = p_doc->createTextNode(U2X(enable_sound ? "yes" : "no"));
    p_child->appendChild(p_text);
    p_root->appendChild(p_child);

    // Write it out to disk
    LocalFileFormatTarget target(U2X(Pathmap::GetConfigPath().utf8_str()));
    DOMLSSerializer* p_serializer = p_impl->createLSSerializer();
    DOMLSOutput*     p_output     = p_impl->createLSOutput();

    DOMConfiguration* p_serialconfig = p_serializer->getDomConfig();
    p_serialconfig->setParameter(XMLUni::fgDOMWRTFormatPrettyPrint, true);
    p_serializer->setNewLine(U2X("\n"));

    p_output->setByteStream(&target);
    p_serializer->write(p_doc, p_output);

    p_output->release();
    p_serializer->release();
    p_doc->release();
}
