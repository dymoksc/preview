#include "SocketWrapper.h"

#include <cstring>
#include <stdexcept>
#include <fcntl.h>
#include <sys/socket.h>
#include <unistd.h>
#include <memory>

namespace system_wrappers {

SocketWrapper::SocketWrapper() : fd(socket(AF_INET, SOCK_STREAM, 0)) {
  if (fd == -1) {
    throw std::runtime_error(std::string("Error in socket: ") + strerror(errno));
  }

  int enable = 1;
  if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0) {
    throw std::runtime_error(std::string("Error in setsockopt(SO_REUSEADDR) : ") + strerror(errno));
  }

  sockAddrIn.sin_family = AF_INET;
  sockAddrIn.sin_addr.s_addr = htonl(INADDR_ANY);
  sockAddrIn.sin_port = htons(port);
  if (bind(fd, reinterpret_cast<sockaddr*>(&sockAddrIn), sizeof(sockAddrIn)) == -1) {
    throw std::runtime_error(std::string("Error in socket: ") + strerror(errno));
  }
  if (listen(fd, maxQueueConnections) == -1) {
    throw std::runtime_error(std::string("Error in listen: ") + strerror(errno));
  }
  if (fcntl(fd, F_SETFL, fcntl(fd, F_GETFL, 0) | O_NONBLOCK) == -1) {
    throw std::runtime_error(std::string("Error in fcntl: ") + strerror(errno));
  }
}

SocketWrapper::~SocketWrapper() {
  close(fd);
}

int SocketWrapper::getFd() const {
  return fd;
}

sockaddr_in& SocketWrapper::getSockAddrIn() {
  return sockAddrIn;
}

std::shared_ptr<SocketConnectionWrapper> SocketWrapper::accept() const {
  static socklen_t clientAddrLength = sizeof(sockaddr_in);
  sockaddr_in clientAddr = {};
  int acceptedSocketFd;
  if ((acceptedSocketFd = ::accept(fd, reinterpret_cast<sockaddr*>(&clientAddr), &clientAddrLength)) == -1) {
    throw std::runtime_error(std::string("Error in accept: ") + strerror(errno));
  }

  return std::make_shared<SocketConnectionWrapper>(acceptedSocketFd, clientAddr);
}

}
