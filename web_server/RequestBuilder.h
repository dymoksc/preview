#ifndef PREVIEW_REQUESTBUILDER_H
#define PREVIEW_REQUESTBUILDER_H

#include "web_server/container/Request.h"

namespace web_server {

class RequestBuilder {
public:
  void setFirstLine(const Request::FirstLineData& firstLine);
  void addHeader(const Request::Header& header);
  void setBody(const Request::Body& body);
  [[nodiscard]] Request buildRequest() const;

private:
  Request::Method requestMethod = Request::Method::GET;
  Request::Url requestUrl;
  Request::Protocol requestProtocol = Request::Protocol::HTTP_1_1;
  Request::Headers requestHeaders;
  Request::Body requestBody;
};

}


#endif //PREVIEW_REQUESTBUILDER_H
