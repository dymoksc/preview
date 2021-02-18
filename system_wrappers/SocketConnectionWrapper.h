#ifndef PREVIEW_SOCKETCONNECTIONWRAPPER_H
#define PREVIEW_SOCKETCONNECTIONWRAPPER_H

#include <string>

#include <netinet/in.h>

namespace system_wrappers {

class SocketConnectionWrapper {
public:
  explicit SocketConnectionWrapper(int fd, const sockaddr_in& clientAddr);
  ~SocketConnectionWrapper();
  std::string receive();
  void send(const std::string& data);
  [[nodiscard]] const sockaddr_in& getClientAddr() const;

private:
  static const size_t readingBufferSize = 1024;

  int fd;
  char readingBuffer[readingBufferSize];
  sockaddr_in clientAddr = {};
};

}

#endif //PREVIEW_SOCKETCONNECTIONWRAPPER_H
