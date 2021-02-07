#ifndef PREVIEW_REQUEST_H
#define PREVIEW_REQUEST_H

#include <string>
#include <map>

namespace web_server {

BETTER_ENUM(RequestMethod, int, GET, POST);
BETTER_ENUM(RequestProtocol, int, HTTP_1_1, HTTP_2);

struct Request {
  typedef RequestMethod Method;
  typedef RequestProtocol Protocol;
  typedef std::map<std::string, std::string> Headers;
  typedef std::string Body;

  const Method method;
  const std::string url;
  const Protocol protocol;
  const Headers headers;
  const Body body;
};

}

#endif //PREVIEW_REQUEST_H
