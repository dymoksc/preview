#include "ResponseBuilder.h"

namespace web_server {

ResponseBuilder& ResponseBuilder::setStatusCode(const Response::StatusCode& statusCode) {
  responseStatusCode = statusCode;
  return *this;
}

ResponseBuilder& ResponseBuilder::setProtocol(const Response::Protocol& protocol) {
  responseProtocol = protocol;
  return *this;
}

ResponseBuilder& ResponseBuilder::addHeader(const web_server::Response::Header& header) {
  responseHeaders.insert(header);
  return *this;
}

ResponseBuilder& ResponseBuilder::setBody(const Response::Body& body) {
  responseBody = body;
  return *this;
}

Response ResponseBuilder::buildResponse() const {
  return { responseProtocol, responseStatusCode, responseHeaders, responseBody };
}

}

