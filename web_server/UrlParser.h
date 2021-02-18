#ifndef PREVIEW_URLPARSER_H
#define PREVIEW_URLPARSER_H

#include "IUrlParser.h"

#include <regex>
#include <string>

namespace web_server {

class UrlParser : public IUrlParser {
private:
  const int RegexGroupPath = 5;

  std::regex regex;

public:
  UrlParser() : regex(R"(^(.*:\/\/)?(.*?)?(:\d*)?(\/(.*?))?(\?.*?)?(\#.*?)?$)") {}
  ~UrlParser() override = default;
  [[nodiscard]] std::string getPath(const Request::Url& url) const override;
};

}

#endif //PREVIEW_URLPARSER_H
