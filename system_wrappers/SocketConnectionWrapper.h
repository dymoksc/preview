#ifndef PREVIEW_SOCKETCONNECTIONWRAPPER_H
#define PREVIEW_SOCKETCONNECTIONWRAPPER_H

#include <string>

namespace system_wrappers {

class SocketConnectionWrapper {
public:
  explicit SocketConnectionWrapper(int fd);
  ~SocketConnectionWrapper();
  std::string receive();
  void send(const std::string& data);

private:
  static const size_t readingBufferSize = 1024;

  int fd;
  char readingBuffer[readingBufferSize];
};

}

#endif //PREVIEW_SOCKETCONNECTIONWRAPPER_H
