#include <iostream>

#include <arpa/inet.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <libs/di/include/boost/di.hpp>

#include "system_wrappers/EpollWrapper.h"
#include "system_wrappers/SocketConnectionWrapper.h"
#include "threads/ThreadPool.h"
#include "web_server/IRequestParser.h"
#include "web_server/IResponsePrinter.h"
#include "web_server/IUrlParser.h"
#include "web_server/RequestParser.h"
#include "web_server/ResponsePrinter.h"
#include "web_server/Server.h"
#include "web_server/UrlParser.h"

#define PORT 8080
#define MAX_CONNECTIONS_IN_QUEUE 3
#define THREADS_COUNT 3

using std::cout;
using std::cerr;
using std::endl;

int main() {
  cerr << "Loading DI definitions..." << endl;
  const auto injector = boost::di::make_injector(
      boost::di::bind<web_server::IRequestParser>().to<web_server::RequestParser>(),
      boost::di::bind<web_server::IResponsePrinter>().to<web_server::ResponsePrinter>(),
      boost::di::bind<web_server::IUrlParser>().to<web_server::UrlParser>(),
      boost::di::bind<web_server::Server>().to<web_server::Server>()
  );

  cerr << "Creating a thread pool..." << endl;
  threads::ThreadPool threadPool(THREADS_COUNT);

  cerr << "Setting up a socket listener..." << endl;
  int socketFd;
  if ((socketFd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
    throw std::runtime_error(std::string("Error in socket: ") + strerror(errno));
  }
  {
    int enable = 1;
    if (setsockopt(socketFd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0) {
      throw std::runtime_error(std::string("Error in setsockopt(SO_REUSEADDR) : ") + strerror(errno));
    }
  }
  sockaddr_in sockAddrIn = {};
  sockAddrIn.sin_family = AF_INET;
  sockAddrIn.sin_addr.s_addr = htonl(INADDR_ANY);
  sockAddrIn.sin_port = htons(PORT);
  if (bind(socketFd, reinterpret_cast<sockaddr*>(&sockAddrIn), sizeof(sockAddrIn)) == -1) {
    throw std::runtime_error(std::string("Error in socket: ") + strerror(errno));
  }
  if (listen(socketFd, MAX_CONNECTIONS_IN_QUEUE) == -1) {
    throw std::runtime_error(std::string("Error in listen: ") + strerror(errno));
  }
  if (fcntl(socketFd, F_SETFL, fcntl(socketFd, F_GETFL, 0) | O_NONBLOCK) == -1) {
    throw std::runtime_error(std::string("Error in fcntl: ") + strerror(errno));
  }

  cerr << "Setting up epoll..." << endl;
  system_wrappers::EpollWrapper epollWrapper(socketFd);

  cerr << "Entering the main loop..." << endl;
  while (true) {
    auto epollEventsIterators = epollWrapper.wait();
    std::for_each(
        epollEventsIterators.first,
        epollEventsIterators.second,
        [&socketFd, &sockAddrIn, &injector](const epoll_event& epollEvent) -> void {
            cerr << "Event on fd #" << epollEvent.data.fd << endl;
            if (epollEvent.events & EPOLLIN) {
              // Accepting connection
              sockaddr_in clientAddr = {};
              socklen_t clientAddrLength = sizeof(clientAddr);
              int acceptedSocketFd;
              if ((acceptedSocketFd = accept(socketFd, reinterpret_cast<sockaddr*>(&sockAddrIn), &clientAddrLength)) == -1) {
                throw std::runtime_error(std::string("Error in accept: ") + strerror(errno));
              }
              system_wrappers::SocketConnectionWrapper socketConnectionWrapper(acceptedSocketFd);

              cout << "Client connected " << inet_ntoa(clientAddr.sin_addr) << endl;

              auto server = injector.create<std::shared_ptr<web_server::Server>>();

              std::string request = socketConnectionWrapper.receive();
              std::string response = server->getResponse(request);
              socketConnectionWrapper.send(response);

              close(acceptedSocketFd);
            }
        }
    );
  }

  // bind(socketDescriptor, )

  // auto start = high_resolution_clock::now();
  //
  // {
  //   threads::ThreadPool threadPool(10);
  //   for (int i = 0; i < 3; ++i) {
  //     threadPool.addTask([]() -> void {
  //       cout << std::this_thread::get_id() << " in\n";
  //       std::this_thread::sleep_for(1s);
  //       cout << std::this_thread::get_id() << " out\n";
  //     });
  //   }
  //   threadPool.waitForAllTasks();
  // }
  //
  // auto finish = high_resolution_clock::now();
  // cout << "Execution time: " << duration_cast<milliseconds>(finish - start).count() << "ms" << endl;

  return 0;
}
