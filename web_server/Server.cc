#include "Server.h"

#include <fstream>
#include <sstream>

#include "web_server/IRequestParser.h"
#include "web_server/IResponsePrinter.h"
#include "web_server/IUrlParser.h"
#include "web_server/ResponseBuilder.h"

namespace web_server {

const char* Server::rootDir = "/home/gleb/public_html/";

Server::Server(const IRequestParser& RequestParser, const IResponsePrinter& ResponsePrinter, const IUrlParser& UrlParser) :
    requestParser(RequestParser), responsePrinter(ResponsePrinter), urlParser(UrlParser) {}

template<>
Response Server::process<Request::Method::GET>(const Request& request) const {
  const std::string& path = urlParser.getPath(request.url);
  std::ifstream inputFileStream(rootDir + path);
  ResponseBuilder responseBuilder;
  responseBuilder.setProtocol(request.protocol);
  if (inputFileStream.good()) {
    responseBuilder.setStatusCode(Response::StatusCode::Ok);
    responseBuilder.addHeader({ "Content-Type", "text/html; charset=utf-8" });
    std::stringstream bodyStream;
    bodyStream << inputFileStream.rdbuf();
    responseBuilder.setBody(bodyStream.str());
  } else {
    responseBuilder.setStatusCode(Response::StatusCode::NotFound);
    responseBuilder.addHeader({ "Content-Type", "text/plain; charset=utf-8" });
    responseBuilder.setBody("Page was not found\n");
  }

  return responseBuilder.buildResponse();
}

Response Server::process(const Request& request) const {
  switch (request.method) {
    case Request::Method::GET:
      return process<Request::Method::GET>(request);

    default:
      return ResponseBuilder()
        .setProtocol(request.protocol)
        .setStatusCode(Response::StatusCode::MethodNotAllowed)
        .addHeader({ "Content-Type", "text/plain; charset=utf-8" })
        .setBody("Method is not supported")
        .buildResponse();
  }
}

std::string Server::getResponse(const std::string& rawRequest) const {
  return responsePrinter.print(process(requestParser.parse(rawRequest)));
}

}
