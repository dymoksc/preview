#ifndef PREVIEW_HTTPTRANSPORTENUMNAMES_H
#define PREVIEW_HTTPTRANSPORTENUMNAMES_H

#include <map>
#include <string>

#include "web_server/container/Protocol.h"
#include "web_server/container/Request.h"
#include "web_server/container/Response.h"

namespace web_server {

class HttpTransportEnumNames {
public:
  const std::map<common::Protocol, std::string> protocolNames;
  const std::map<Request::Method, std::string> requestMethodNames;
  const std::map<Response::StatusCode, std::string> statusReasonMessages;

  HttpTransportEnumNames();
};

}


#endif //PREVIEW_HTTPTRANSPORTENUMNAMES_H
