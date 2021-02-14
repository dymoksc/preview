#ifndef PREVIEW_HEADER_H
#define PREVIEW_HEADER_H

#include <string>
#include <map>

namespace web_server::common {

typedef std::pair<std::string, std::string> Header;
typedef std::map<std::string, std::string> Headers;

}

#endif //PREVIEW_HEADER_H
