#ifndef PREVIEW_REQUEST_H
#define PREVIEW_REQUEST_H

#include <string>

#include "web_server/container/Header.h"
#include "web_server/container/Protocol.h"

namespace web_server {

struct Request {
  typedef std::string Url;
  typedef common::Protocol Protocol;
  typedef common::Header Header;
  typedef common::Headers Headers;
  typedef std::string Body;

  enum class Method { GET, POST };

  struct RequestLineData {
    const Method method;
    const Url url;
    const Protocol protocol;
  };

  const Method method;
  const Url url;
  const Protocol protocol;
  const Headers headers;
  const Body body;
};

}

#endif //PREVIEW_REQUEST_H
