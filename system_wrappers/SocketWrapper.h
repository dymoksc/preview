#ifndef PREVIEW_SOCKETWRAPPER_H
#define PREVIEW_SOCKETWRAPPER_H

#include <netinet/in.h>
#include <memory>

#include "system_wrappers/SocketConnectionWrapper.h"

namespace system_wrappers {

class SocketWrapper {
public:
  SocketWrapper();
  ~SocketWrapper();
  [[nodiscard]] int getFd() const;
  sockaddr_in& getSockAddrIn();
  std::shared_ptr<SocketConnectionWrapper> accept() const;

private:
  static const int port = 8080;
  static const int maxQueueConnections = 3;

  int fd;
  sockaddr_in sockAddrIn = {};
};

}

#endif //PREVIEW_SOCKETWRAPPER_H
