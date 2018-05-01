#include "level_loader.hpp"
#include "../level.hpp"
#include "../xerces_helpers.hpp"
#include <xercesc/sax2/Attributes.hpp>

using namespace std;
using namespace xercesc;
using namespace TSC;

LevelLoader::LevelLoader(Level& level)
    : DefaultHandler(),
      m_level(level)
{
}

void LevelLoader::startElement(const XMLCh* const,
                               const XMLCh* const xlocalname,
                               const XMLCh* const,
                               const Attributes& attributes)
{
    m_chars.clear();
    string localname = X2U(xlocalname);

    if (localname == "level") {
        int enginever = stoi(X2U(attributes.getValue(U2X("enginever"))));
        if (enginever != LEVEL_ENGINE_VERSION)
            throw(runtime_error("Unsupported level engine version"));

        m_level.m_width           = stoi(X2U(attributes.getValue(U2X("width"))));
        m_level.m_height          = stoi(X2U(attributes.getValue(U2X("height"))));
        m_level.m_fixed_cam_speed = stoi(X2U(attributes.getValue(U2X("fixedcamspeed"))));
        m_level.m_music           = X2U(attributes.getValue(U2X("music")));
    }
    else if (localname == "player") {
        // TODO
    }
    else if (localname == "ground") {
        m_current_tileset = X2U(attributes.getValue(U2X("tileset")));
        int x = stoi(X2U(attributes.getValue(U2X("x"))));
        int y = stoi(X2U(attributes.getValue(U2X("y"))));

        m_level.m_grounds.resize(m_level.m_grounds.size() + 1);
        m_level.m_grounds[m_level.m_grounds.size()-1].setPosition(x, y);
    }
    else if (localname == "field") {
        int relx = stoi(X2U(attributes.getValue(U2X("relx"))));
        int rely = stoi(X2U(attributes.getValue(U2X("rely"))));
        int tid  = stoi(X2U(attributes.getValue(U2X("tid"))));
        m_current_fields.emplace_back(relx, rely, tid);
    }
}

void LevelLoader::endElement(const XMLCh* const,
                             const XMLCh* const xlocalname,
                             const XMLCh* const)
{
    string localname = X2U(xlocalname);

    if (localname == "ground") {
        m_level.m_grounds[m_level.m_grounds.size()-1].reset(m_current_tileset, m_current_fields);
        m_current_tileset.clear();
        m_current_fields.clear();
    }
}

void LevelLoader::characters(const XMLCh* const chars, const XMLSize_t)
{
    m_chars.append(X2U(chars));
}
