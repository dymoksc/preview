#ifndef PREVIEW_IREQUESTBUILDER_H
#define PREVIEW_IREQUESTBUILDER_H

#include "web_server/container/Request.h"

namespace web_server {

class IRequestBuilder {
public:
  virtual ~IRequestBuilder() = default;
  virtual void setFirstLine(const Request::RequestLineData& firstLine) = 0;
  virtual void addHeader(const Request::Header& header) = 0;
  virtual void setBody(const Request::Body& body) = 0;
  [[nodiscard]] virtual Request buildRequest() const = 0;
};

}

#endif //PREVIEW_IREQUESTBUILDER_H
