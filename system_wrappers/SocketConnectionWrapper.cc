#include "SocketConnectionWrapper.h"

#include <unistd.h>
#include <sstream>
#include <sys/socket.h>

namespace system_wrappers {

SocketConnectionWrapper::SocketConnectionWrapper(int fd) : fd(fd) {}

SocketConnectionWrapper::~SocketConnectionWrapper() {
  close(fd);
}

std::string SocketConnectionWrapper::receive() {
  std::stringstream inputStream;
  int readingResult;
  while ((readingResult = recv(fd, readingBuffer, readingBufferSize, 0)) != 0) {
    if (readingResult == -1) {
      throw std::runtime_error(std::string("Error in recv: ") + strerror(errno));
    }

    if (readingResult != readingBufferSize) {
      inputStream << std::string(std::begin(readingBuffer), std::begin(readingBuffer) + readingResult);
      break;
    } else {
      inputStream << readingBuffer;
    }
  }

  return inputStream.str();
}

void SocketConnectionWrapper::send(const std::string& data) {
  if (::send(fd, data.c_str(), data.size(), 0) == -1) {
    throw std::runtime_error(std::string("Error in send: ") + strerror(errno));
  }
}

}
