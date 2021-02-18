#include "ResponsePrinter.h"

#include <sstream>

namespace web_server {

std::string web_server::ResponsePrinter::print(const web_server::Response& response) const {
  std::stringstream responseStream;
  responseStream << httpTransportEnumNames.protocolNames.at(response.protocol) << " "
                 << static_cast<std::underlying_type_t<Response::StatusCode>>(response.statusCode) << " "
                 << httpTransportEnumNames.statusReasonMessages.at(response.statusCode) << "\r\n"
                 << (!response.body.empty() ? "\r\n" + response.body : "");

  return responseStream.str();
}

}

