#ifndef PREVIEW_RESPONSE_H
#define PREVIEW_RESPONSE_H

#include "web_server/container/Header.h"
#include "web_server/container/Protocol.h"

namespace web_server {

struct Response {
  typedef common::Protocol Protocol;
  typedef common::Header Header;
  typedef common::Headers Headers;
  typedef std::string Body;

  enum class StatusCode : int { Ok = 200, NotFound = 404, MethodNotAllowed = 405 };

  struct StatusLine {
    Protocol protocol;
    StatusCode statusCode;
  };

  Protocol protocol;
  const StatusCode statusCode;
  const Headers headers;
  const Body body;
};

}

#endif //PREVIEW_RESPONSE_H
