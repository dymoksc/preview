#ifndef PREVIEW_HEADER_H
#define PREVIEW_HEADER_H

#include <string>
#include <map>

namespace web_server::common {

typedef std::map<std::string, std::string> Headers;
typedef std::map<std::string, std::string>::value_type Header;

}

#endif //PREVIEW_HEADER_H
