#include "RequestParser.h"

#include <boost/algorithm/string/join.hpp>
#include <boost/range/adaptors.hpp>

#include "web_server/ParsingException.h"
#include "web_server/RequestBuilder.h"

namespace web_server {

template <>
Request::Header RequestParser::parseLine(const std::string& line) const {
  std::smatch matches;
  if (!std::regex_match(line, matches, headerLineRegex)) {
    throw ParsingException("Header line cannot be parsed: '"  + line + "'");
  }

  return { matches.str(1), matches.str(2) };
}

template <>
Request::FirstLineData RequestParser::parseLine(const std::string& line) const {
  std::smatch matches;
  if (!std::regex_match(line, matches, firstLineRegex)) {
    throw ParsingException("First line does not follow the expected format: '" + line + "'");
  }

  return {
      std::find_if(supportedMethods.begin(), supportedMethods.end(), [&matches](const auto& method) { return method.second == matches.str(1); })->first,
      matches.str(2),
      std::find_if(supportedProtocols.begin(), supportedProtocols.end(), [&matches](const auto& protocol) { return protocol.second == matches.str(3); })->first,
  };
}

RequestParser::RequestParser()  :
    supportedProtocols({ { Request::Protocol::HTTP_1_1, "HTTP/1.1"}, { Request::Protocol::HTTP_2, "HTTP/2" } }),
    supportedMethods({ { Request::Method::GET, "GET" }, { Request::Method::POST, "POST" } }),
    firstLineRegex([this]() -> std::regex {
      const std::string& methodsRegexp = boost::algorithm::join(supportedMethods | boost::adaptors::map_values, "|");
      const std::string& protocolsRegexp = boost::algorithm::join(supportedProtocols | boost::adaptors::map_values, "|");

      return std::regex("^(" + methodsRegexp + ") (.+) (" + protocolsRegexp + ")");
    }()),
    headerLineRegex("^(.+?):\\s*(.+)") {}

Request RequestParser::parse(const std::string& rawRequest) const {
  ParsingPhase parsingPhase = ParsingPhase::FirstLine;
  RequestBuilder requestBuilder;
  requestBuilder.addHeader({ "a", "b" });

  std::stringstream requestStream(rawRequest);
  while (parsingPhase != ParsingPhase::Finished) {
    switch (parsingPhase) {
      case ParsingPhase::FirstLine: {
        std::string firstLine;
        getline(requestStream, firstLine);
        requestBuilder.setFirstLine(parseLine<Request::FirstLineData>(firstLine));
        parsingPhase = ParsingPhase::Headers;
        break;
      }

      case ParsingPhase::Headers: {
        std::string headerLine;
        getline(requestStream, headerLine);
        if (headerLine.empty()) {
          parsingPhase = ParsingPhase::Body;
        } else {
          requestBuilder.addHeader(parseLine<Request::Header>(headerLine));
        }
        break;
      }

      case ParsingPhase::Body: {
        std::stringstream bodyStream;
        bodyStream << requestStream.rdbuf();
        requestBuilder.setBody(bodyStream.str());
        parsingPhase = ParsingPhase::Finished;
        break;
      }

      case ParsingPhase::Finished:
        // do nothing
        break;
    }
  }

  return requestBuilder.buildRequest();
}

}
