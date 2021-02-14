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

  std::regex firstLineRegex;
  std::regex headerLineRegex;

  [[nodiscard]] Request::FirstLineData parseFirstLine(const std::string& firstLine) const;
  [[nodiscard]] Request::Header parseHeaderLine(const std::string& headerLine) const;
};

}


#endif //PREVIEW_REQUESTPARSER_H
