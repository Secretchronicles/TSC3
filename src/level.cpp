#include "level.hpp"
#include "pathmap.hpp"
#include "xml_loaders/level_loader.hpp"
#include "xerces_helpers.hpp"
#include <xercesc/sax2/XMLReaderFactory.hpp>
#include <xercesc/framework/LocalFileInputSource.hpp>

using namespace TSC;
using namespace std;

Level::Level(const std::string& relfilename)
{
    using namespace xercesc;

    SAX2XMLReader* p_reader = XMLReaderFactory::createXMLReader();
    p_reader->setFeature(XMLUni::fgSAX2CoreValidation, false);

    LevelLoader handler(*this);
    p_reader->setContentHandler(&handler);
    p_reader->setErrorHandler(&handler);

    LocalFileInputSource source(U2X(Pathmap::GetLevelPath(relfilename).utf8_str()));
    p_reader->parse(source);
}

Level::~Level()
{
}

void Level::Update()
{
}

void Level::Draw(sf::RenderTarget& stage) const
{
    for (const Ground& ground: m_grounds) {
        stage.draw(ground);
    }
}
