#include "RequestParser.h"

namespace web_server {

Request RequestParser::parse(const std::string& rawRequest) const {
  return { Request::Method::GET, "", Request::Protocol::HTTP_2, {}, "" };
}

}
