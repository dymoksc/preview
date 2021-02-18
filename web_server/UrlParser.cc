#include "UrlParser.h"

namespace web_server {

std::string UrlParser::getPath(const web_server::Request::Url& url) const {
  std::smatch matches;

  if (!std::regex_match(url, matches, regex)) {
    return "";
  }

  return matches[RegexGroupPath];
}

}

