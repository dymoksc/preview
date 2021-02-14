#ifndef PREVIEW_IREQUESTPARSER_H
#define PREVIEW_IREQUESTPARSER_H

#include "web_server/container/Request.h"

namespace web_server {

class IRequestParser {
public:
  virtual ~IRequestParser() = default;
  [[nodiscard]] virtual Request parse(const std::string& rawRequest) const = 0;
};

}

#endif //PREVIEW_IREQUESTPARSER_H
