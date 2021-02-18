#include "EpollWrapper.h"

#include <cstring>
#include <stdexcept>
#include <unistd.h>

namespace system_wrappers {

EpollWrapper::EpollWrapper(int socketFd) : fd(epoll_create(maxConnections)) {
  if (fd == -1) {
    throw std::runtime_error(std::string("Error in epoll_create: ") + strerror(errno));
  }

  epoll_event eventSettings = { epollEventsSettings, {} };
  eventSettings.data.fd = socketFd;
  if (epoll_ctl(fd, EPOLL_CTL_ADD, socketFd, &eventSettings) == -1) {
    throw std::runtime_error(std::string("Error in epoll_ctl: ") + strerror(errno));
  }
}

EpollWrapper::~EpollWrapper() {
  close(fd);
}

std::pair<const epoll_event*, const epoll_event*> EpollWrapper::wait() {
  int readyDescriptorsCount;
  readyDescriptorsCount = epoll_wait(fd, epollEvents, maxEvents, -1);

  if (readyDescriptorsCount == -1) {
    throw std::runtime_error(std::string("Error in epoll_wait: ") + strerror(errno));
  }

  return std::make_pair(std::begin(epollEvents), std::begin(epollEvents) + readyDescriptorsCount);
}

}
