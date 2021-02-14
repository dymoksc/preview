#ifndef PREVIEW_REQUEST_H
#define PREVIEW_REQUEST_H

#include <string>

#include "web_server/container/Header.h"

namespace web_server {

BETTER_ENUM(RequestMethod, int, GET, POST)
BETTER_ENUM(RequestProtocol, int, HTTP_1_1, HTTP_2)

struct Request {
  typedef RequestMethod Method;
  typedef std::string Url;
  typedef RequestProtocol Protocol;
  typedef common::Header Header;
  typedef common::Headers Headers;
  typedef std::string Body;

  struct FirstLineData {
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
