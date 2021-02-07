#ifndef PREVIEW_REQUESTPARSER_H
#define PREVIEW_REQUESTPARSER_H

#include "IRequestParser.h"

namespace web_server {

class RequestParser : public IRequestParser {
public:
  ~RequestParser() override = default;
  [[nodiscard]] Request parse(const std::string& rawRequest) const override;
};

}


#endif //PREVIEW_REQUESTPARSER_H
