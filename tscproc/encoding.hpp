#ifndef TSCPROC_ENCODING_HPP
#define TSCPROC_ENCODING_HPP
#include <string>
#include <memory>
#include <xercesc/util/XMLString.hpp>

std::string xstr_to_utf8(const XMLCh* xstr);
std::unique_ptr<XMLCh[]> utf8_to_xstr(const std::string& utf8);

#endif /* TSCPROC_ENCODING_HPP */
