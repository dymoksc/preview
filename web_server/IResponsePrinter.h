#ifndef PREVIEW_IRESPONSEPRINTER_H
#define PREVIEW_IRESPONSEPRINTER_H

#include <string>

namespace web_server {

class Response;

class IResponsePrinter {
public:
  virtual ~IResponsePrinter() = default;
  [[nodiscard]] virtual std::string print(const Response& response) const = 0;
};

}


#endif //PREVIEW_IRESPONSEPRINTER_H
