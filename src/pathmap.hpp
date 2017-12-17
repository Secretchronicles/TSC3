#ifndef TSC_PATHMAP_HPP
#define TSC_PATHMAP_HPP
#include <pathie/path.hpp>

namespace TSC {

    class Pathmap {
    public:
        Pathmap();

        Pathie::Path GetConfigPath() const;
    };

}

#endif /* TSC_PATHMAP_HPP */