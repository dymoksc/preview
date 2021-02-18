#ifndef PREVIEW_RESPONSEPRINTER_H
#define PREVIEW_RESPONSEPRINTER_H

#include "IResponsePrinter.h"

#include "web_server/HttpTransportEnumNames.h"
#include "web_server/container/Response.h"

namespace web_server {

class ResponsePrinter : public IResponsePrinter {
public:
  ~ResponsePrinter() override = default;
  [[nodiscard]] std::string print(const Response& response) const override;

private:
  const HttpTransportEnumNames httpTransportEnumNames;
};

}

#endif //PREVIEW_RESPONSEPRINTER_H
