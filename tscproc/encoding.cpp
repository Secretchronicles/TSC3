#include "encoding.hpp"
#include <xercesc/util/TransService.hpp>
#include <cstring>

using namespace std;
using namespace xercesc;

/**
 * Converts the given Xerces-C string into a UTF-8 string.
 */
string xstr_to_utf8(const XMLCh* xstr)
{
    TranscodeToStr transcoder(xstr, "UTF-8");
    return string(reinterpret_cast<const char*>(transcoder.str()));
}

/**
 * Converts the given UTF-8 string into a Xerces-C string.
 * The return value of this method is guaranteed to have
 * a terminal NUL.
 */
unique_ptr<XMLCh[]> utf8_to_xstr(const std::string& utf8)
{
    TranscodeFromStr transcoder(reinterpret_cast<const XMLByte*>(utf8.c_str()), utf8.length(), "UTF-8");
    unique_ptr<XMLCh[]> buf(new XMLCh[transcoder.length()+1]);

    memset(buf.get(), '\0', sizeof(XMLCh) * (transcoder.length() + 1));
    memcpy(buf.get(), transcoder.str(), sizeof(XMLCh) * transcoder.length());

    return buf;
}
