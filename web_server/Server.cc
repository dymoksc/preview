#include "Server.h"

#include <fstream>
#include <sstream>
#include <thread>

#include "web_server/IRequestParser.h"
#include "web_server/IResponsePrinter.h"
#include "web_server/IUrlParser.h"
#include "web_server/ResponseBuilder.h"

namespace web_server {

const char* Server::rootDir = "~/public_html/";

Server::Server(const IRequestParser& RequestParser, const IResponsePrinter& ResponsePrinter, const IUrlParser& UrlParser) :
    requestParser(RequestParser), responsePrinter(ResponsePrinter), urlParser(UrlParser) {}

template<>
void Server::process<Request::Method::GET>(const Request& request, ResponseBuilder& responseBuilder) const {
  const std::string& path = urlParser.getPath(request.url);
  std::ifstream inputFileStream(rootDir + path);
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
}

Response Server::process(const Request& request) const {
  ResponseBuilder responseBuilder;
  responseBuilder.addHeader(
      {
        "X-Thread",
        []() -> std::string {
          std::stringstream ss;
          ss << std::this_thread::get_id();

          return ss.str();
        }()
      }
  );

  switch (request.method) {
    case Request::Method::GET:
      process<Request::Method::GET>(request, responseBuilder);
      break;

    default: {
      responseBuilder
        .setProtocol(request.protocol)
        .setStatusCode(Response::StatusCode::MethodNotAllowed)
        .addHeader({ "Content-Type", "text/plain; charset=utf-8" })
        .setBody("Method is not supported");
    }
  }

  return responseBuilder.buildResponse();
}

std::string Server::getResponse(const std::string& rawRequest) const {
  return responsePrinter.print(process(requestParser.parse(rawRequest)));
}

}
