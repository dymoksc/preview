#ifndef PREVIEW_SERVER_H
#define PREVIEW_SERVER_H

#include <string>

#include "web_server/container/Response.h"
#include "web_server/container/Request.h"

namespace web_server {

class IRequestParser;
class IResponsePrinter;
class IUrlParser;

class Server {
public:
  Server(const IRequestParser& RequestParser, const IResponsePrinter& ResponsePrinter, const IUrlParser& UrlParser);
  [[nodiscard]] std::string getResponse(const std::string& rawRequest) const;

private:
  static const char* rootDir;

  const IRequestParser& requestParser;
  const IResponsePrinter& responsePrinter;
  const IUrlParser& urlParser;

  template <Request::Method method>
  [[nodiscard]] Response process(const Request& request) const;
  [[nodiscard]] Response process(const Request& request) const;
};

}

#endif //PREVIEW_SERVER_H
