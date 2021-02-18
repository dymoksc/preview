#ifndef PREVIEW_IURLPARSER_H
#define PREVIEW_IURLPARSER_H

#include <string>

#include "web_server/container/Request.h"

namespace web_server {

class IUrlParser {
public:
  virtual ~IUrlParser() = default;
  virtual std::string getPath(const Request::Url& url) const = 0;
};

}

#endif //PREVIEW_IURLPARSER_H
