#ifndef TSC_LEVEL_LOADER_HPP
#define TSC_LEVEL_LOADER_HPP
#include <xercesc/sax2/DefaultHandler.hpp>
#include "../ground.hpp"

namespace TSC {

    class Level;

    class LevelLoader: public xercesc::DefaultHandler
    {
    public:
        const int LEVEL_ENGINE_VERSION = 300;

        LevelLoader(Level& level);
        virtual void startElement(const XMLCh* const,
                                  const XMLCh* const,
                                  const XMLCh* const,
                                  const xercesc::Attributes&);

        virtual void endElement(const XMLCh* const,
                                const XMLCh* const xlocalname,
                                const XMLCh* const);

        virtual void characters(const XMLCh* const chars, const XMLSize_t);

    private:
        Level& m_level;
        std::string m_chars;
        std::string m_current_tileset;
        std::vector<Field> m_current_fields;
    };

}

#endif /* TSC_LEVEL_LOADER_HPP */
