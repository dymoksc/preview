#ifndef PREVIEW_RESPONSEBUILDER_H
#define PREVIEW_RESPONSEBUILDER_H

#include "IResponseBuilder.h"

#include "web_server/container/Response.h"

namespace web_server {

class ResponseBuilder : public IResponseBuilder {
public:
  ~ResponseBuilder() override = default;
  ResponseBuilder& setStatusCode(const Response::StatusCode& statusCode) override;
  ResponseBuilder& setProtocol(const Response::Protocol& protocol) override;
  ResponseBuilder& addHeader(const Response::Header& header) override;
  ResponseBuilder& setBody(const Response::Body& body) override;
  [[nodiscard]] Response buildResponse() const override;

private:
  Response::Protocol responseProtocol;
  Response::StatusCode responseStatusCode;
  Response::Headers responseHeaders;
  Response::Body responseBody;
};

}

#endif //PREVIEW_RESPONSEBUILDER_H
