#include "parser.hpp"
#include "encoding.hpp"
#include <memory>
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <xercesc/sax2/DefaultHandler.hpp>
#include <xercesc/sax2/SAX2XMLReader.hpp>
#include <xercesc/sax2/Attributes.hpp>
#include <xercesc/sax2/XMLReaderFactory.hpp>

using namespace std;
using namespace xercesc;

namespace {

    class TilesetHandler: public xercesc::DefaultHandler
    {
    public:
        void startElement(const XMLCh* const xuri,
                          const XMLCh* const xlocalname,
                          const XMLCh* const xqname,
                          const Attributes& attrs)
            {
                std::string localname(xstr_to_utf8(xlocalname));
                if (localname == "colrect") {
                    string x(xstr_to_utf8(attrs.getValue(utf8_to_xstr("x").get())));
                    string y(xstr_to_utf8(attrs.getValue(utf8_to_xstr("y").get())));
                    string w(xstr_to_utf8(attrs.getValue(utf8_to_xstr("width").get())));
                    string h(xstr_to_utf8(attrs.getValue(utf8_to_xstr("height").get())));

                    boxes.push_back(move(make_tuple(atoi(x.c_str()), atoi(y.c_str()), atoi(w.c_str()), atoi(h.c_str()))));
                }
            }

        vector<tuple<int, int, int, int>> boxes;
    };

}

vector<tuple<int, int, int, int>> parse_tileset(const string& path)
{
    unique_ptr<SAX2XMLReader> p_parser(XMLReaderFactory::createXMLReader());
    p_parser->setFeature(XMLUni::fgSAX2CoreValidation, false);

    TilesetHandler handler;
    p_parser->setContentHandler(&handler);
    p_parser->setErrorHandler(&handler);

    try {
        p_parser->parse(utf8_to_xstr(path).get());
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

    return handler.boxes;
}
