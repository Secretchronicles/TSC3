/* TSC is a two-dimensional jump’n’run platform game.
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
 */

#include "parser.hpp"
#include "encoding.hpp"
#include <memory>
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/sax2/DefaultHandler.hpp>
#include <xercesc/sax2/SAX2XMLReader.hpp>
#include <xercesc/sax2/Attributes.hpp>
#include <xercesc/sax2/XMLReaderFactory.hpp>
#include <xercesc/framework/StdInInputSource.hpp>

using namespace std;
using namespace xercesc;

namespace {

    class TilesetHandler: public xercesc::DefaultHandler
    {
    public:
        void startElement(const XMLCh* const,
                          const XMLCh* const xlocalname,
                          const XMLCh* const,
                          const Attributes& attrs)
            {
                std::string localname(xstr_to_utf8(xlocalname));
                if (localname == "colrect") {
                    string x(xstr_to_utf8(attrs.getValue(utf8_to_xstr("x").get())));
                    string y(xstr_to_utf8(attrs.getValue(utf8_to_xstr("y").get())));
                    string w(xstr_to_utf8(attrs.getValue(utf8_to_xstr("width").get())));
                    string h(xstr_to_utf8(attrs.getValue(utf8_to_xstr("height").get())));

                    boxes.push_back(move(bbox{atoi(x.c_str()), atoi(y.c_str()), atoi(w.c_str()), atoi(h.c_str())}));
                }
            }

        vector<bbox> boxes;
    };

}

/**
 * Parses the XML tileset metadata in the given file and returns
 * a list of collision rectangles found. If `path` is empty, reads
 * the XML from standard input instead.
 */
vector<bbox> parse_tileset_metadata(const string& path)
{
    XMLPlatformUtils::Initialize();

    // unique_ptr<SAX2XMLReader> p_parser(XMLReaderFactory::createXMLReader());
    SAX2XMLReader* p_parser = XMLReaderFactory::createXMLReader();
    p_parser->setFeature(XMLUni::fgSAX2CoreValidation, false);

    TilesetHandler handler;
    p_parser->setContentHandler(&handler);
    p_parser->setErrorHandler(&handler);

    try {
        if (path.empty()) { // Read from standard input requested
            StdInInputSource source;
            p_parser->parse(source);
        }
        else { // Read from file requested
            p_parser->parse(utf8_to_xstr(path).get());
        }
    }
    catch (const XMLException& err) {
        cerr << "Error: " << xstr_to_utf8(err.getMessage()) << endl;
        exit(2);
    }
    catch (const SAXParseException& err) {
        cerr << "Error: " << xstr_to_utf8(err.getMessage()) << endl;
        exit(2);
    }
    catch (...) {
        cerr << "Unknown error on parsing the XML." << endl;
        exit(2);
    }

    XMLPlatformUtils::Terminate();
    return handler.boxes;
}
