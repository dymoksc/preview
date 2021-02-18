#ifndef PREVIEW_REQUESTBUILDER_H
#define PREVIEW_REQUESTBUILDER_H

#include "IRequestBuilder.h"

namespace web_server {

class RequestBuilder : public IRequestBuilder {
public:
  ~RequestBuilder() override = default;
  void setFirstLine(const Request::RequestLineData& firstLine) override;
  void addHeader(const Request::Header& header) override;
  void setBody(const Request::Body& body) override;
  [[nodiscard]] Request buildRequest() const override;

private:
  Request::Method requestMethod = Request::Method::GET;
  Request::Url requestUrl;
  Request::Protocol requestProtocol = Request::Protocol::HTTP_1_1;
  Request::Headers requestHeaders;
  Request::Body requestBody;
};

}


#endif //PREVIEW_REQUESTBUILDER_H
