#ifndef PREVIEW_REQUESTPARSER_H
#define PREVIEW_REQUESTPARSER_H

#include "IRequestParser.h"

#include <regex>
#include <map>

namespace web_server {

class RequestParser : public IRequestParser {
public:
  RequestParser();
  ~RequestParser() override = default;
  [[nodiscard]] Request parse(const std::string& rawRequest) const override;

private:
  enum class ParsingPhase { FirstLine, Headers, Body, Finished };

  const std::map<Request::Protocol, std::string> supportedProtocols;
  const std::map<Request::Method, std::string> supportedMethods;
  const std::regex firstLineRegex;
  const std::regex headerLineRegex;

  template <typename T>
  [[nodiscard]] T parseLine(const std::string& line) const;
};

}


#endif //PREVIEW_REQUESTPARSER_H
