#include "RequestBuilder.h"

namespace web_server {

void RequestBuilder::setFirstLine(const web_server::Request::RequestLineData& firstLine) {
  requestMethod = firstLine.method;
  requestUrl = firstLine.url;
  requestProtocol = firstLine.protocol;
}

void RequestBuilder::addHeader(const Request::Header& header) {
  requestHeaders.insert(header);
}

void RequestBuilder::setBody(const Request::Body& body) {
  requestBody = body;
}

Request RequestBuilder::buildRequest() const {
  return { requestMethod, requestUrl, requestProtocol, requestHeaders, requestBody };
}

}
