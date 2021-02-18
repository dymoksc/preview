#ifndef PREVIEW_EPOLLWRAPPER_H
#define PREVIEW_EPOLLWRAPPER_H

#include <utility>

#include <sys/epoll.h>

namespace system_wrappers {

class EpollWrapper {
public:
  explicit EpollWrapper(int sockedFd);
  ~EpollWrapper();
  std::pair<const epoll_event*, const epoll_event*> wait();

private:
  static const int maxConnections = 3;
  static const int maxEvents = 3;
  static const uint32_t epollEventsSettings = EPOLLIN;

  int fd;
  epoll_event epollEvents[maxEvents] = {};
};

}

#endif //PREVIEW_EPOLLWRAPPER_H
