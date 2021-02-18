#ifndef PREVIEW_IRESPONSEBUILDER_H
#define PREVIEW_IRESPONSEBUILDER_H

#include "web_server/container/Response.h"

namespace web_server {

class IResponseBuilder {
public:
  virtual ~IResponseBuilder() = default;
  virtual IResponseBuilder& setStatusCode(const Response::StatusCode& statusCode) = 0;
  virtual IResponseBuilder& setProtocol(const Response::Protocol& protocol) = 0;
  virtual IResponseBuilder& addHeader(const Response::Header& header) = 0;
  virtual IResponseBuilder& setBody(const Response::Body& body) = 0;
  [[nodiscard]] virtual Response buildResponse() const = 0;
};

}

#endif //PREVIEW_IRESPONSEBUILDER_H
