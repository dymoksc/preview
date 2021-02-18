#include <iostream>
#include <chrono>

#include <arpa/inet.h>
#include <boost/asio/ip/tcp.hpp>
#include <fcntl.h>
#include <netinet/in.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <sstream>
#include <libs/di/include/boost/di.hpp>

#include "threads/ThreadPool.h"
#include "web_server/IRequestParser.h"
#include "web_server/IResponsePrinter.h"
#include "web_server/IUrlParser.h"
#include "web_server/RequestParser.h"
#include "web_server/ResponsePrinter.h"
#include "web_server/Server.h"
#include "web_server/UrlParser.h"

#define MAX_CONNECTIONS 3
#define MAX_EPOLL_EVENTS 3
#define PORT 8080
#define MAX_CONNECTIONS_IN_QUEUE 3
#define BUFFER_SIZE 9

using namespace std::chrono;

int main() {
  const auto injector = boost::di::make_injector(
      boost::di::bind<web_server::IRequestParser>().to<web_server::RequestParser>(),
      boost::di::bind<web_server::IResponsePrinter>().to<web_server::ResponsePrinter>(),
      boost::di::bind<web_server::IUrlParser>().to<web_server::UrlParser>(),
      boost::di::bind<web_server::Server>().to<web_server::Server>()
  );

  // Creating epoll
  int epollFd = epoll_create(MAX_CONNECTIONS);
  if (epollFd == -1) {
    throw std::runtime_error(std::string("Error in epoll_create: ") + strerror(errno));
  }

  // Creating socket
  int socketFd;
  if ((socketFd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
    throw std::runtime_error(std::string("Error in socket: ") + strerror(errno));
  }
  int enable = 1;
  if (setsockopt(socketFd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0) {
    throw std::runtime_error(std::string("Error in setsockopt(SO_REUSEADDR) : ") + strerror(errno));
  }

  // Setting up an address
  sockaddr_in sockAddrIn = {};
  sockAddrIn.sin_family = AF_INET;
  sockAddrIn.sin_addr.s_addr = htonl(INADDR_ANY);
  sockAddrIn.sin_port = htons(PORT);
  if (bind(socketFd, reinterpret_cast<sockaddr*>(&sockAddrIn), sizeof(sockAddrIn)) == -1) {
    throw std::runtime_error(std::string("Error in socket: ") + strerror(errno));
  }

  // Checking a listen
  if (listen(socketFd, MAX_CONNECTIONS_IN_QUEUE) == -1) {
    throw std::runtime_error(std::string("Error in listen: ") + strerror(errno));
  }

  // Setting non-blocking
  if (fcntl(socketFd, F_SETFL, fcntl(socketFd, F_GETFL, 0) | O_NONBLOCK) == -1) {
    throw std::runtime_error(std::string("Error in fcntl: ") + strerror(errno));
  }

  // Setting up an EPOLLIN handler
  {
    epoll_event eventSettings = { EPOLLIN, {} };
    eventSettings.data.fd = socketFd;
    if (epoll_ctl(epollFd, EPOLL_CTL_ADD, socketFd, &eventSettings) == -1) {
      throw std::runtime_error(std::string("Error in epoll_ctl: ") + strerror(errno));
    }
  }

  // Epoll main loop
  int readyDescriptorsCount;
  epoll_event currentEvents[MAX_EPOLL_EVENTS];
  while (true) {
    if ((readyDescriptorsCount = epoll_wait(epollFd, currentEvents, MAX_EPOLL_EVENTS, -1)) == -1) {
      throw std::runtime_error(std::string("Error in epoll_wait: ") + strerror(errno));
    }

    for (int i = 0; i < readyDescriptorsCount; ++i) {
      std::cout << "Event " << currentEvents[i].events << std::endl;

      if (currentEvents[i].events & EPOLLIN) {
        // Accepting connection
        sockaddr_in clientAddr = {};
        socklen_t clientAddrLength = sizeof(clientAddr);
        int acceptedSocketFd;
        if ((acceptedSocketFd = accept(socketFd, reinterpret_cast<sockaddr*>(&sockAddrIn), &clientAddrLength)) == -1) {
          throw std::runtime_error(std::string("Error in accept: ") + strerror(errno));
        }

        std::cout << "Client connected " << inet_ntoa(clientAddr.sin_addr) << std::endl;

        char buffer[BUFFER_SIZE];
        std::stringstream inputStream;
        int readingResult;
        while ((readingResult = recv(acceptedSocketFd, buffer, BUFFER_SIZE, 0)) != 0) {
          if (readingResult == -1) {
            throw std::runtime_error("Error in recv");
          }

          if (readingResult != BUFFER_SIZE) {
            inputStream << std::string(std::begin(buffer), std::begin(buffer) + readingResult);
            break;
          } else {
            inputStream << buffer;
          }
        }

        auto server = injector.create<std::shared_ptr<web_server::Server>>();
        std::string response = server->getResponse(inputStream.str());
        send(acceptedSocketFd, response.c_str(), response.size(), 0);

        close(acceptedSocketFd);
      }
    }
  }

  // bind(socketDescriptor, )

  // auto start = high_resolution_clock::now();
  //
  // {
  //   threads::ThreadPool threadPool(10);
  //   for (int i = 0; i < 3; ++i) {
  //     threadPool.addTask([]() -> void {
  //       std::cout << std::this_thread::get_id() << " in\n";
  //       std::this_thread::sleep_for(1s);
  //       std::cout << std::this_thread::get_id() << " out\n";
  //     });
  //   }
  //   threadPool.waitForAllTasks();
  // }
  //
  // auto finish = high_resolution_clock::now();
  // std::cout << "Execution time: " << duration_cast<milliseconds>(finish - start).count() << "ms" << std::endl;

  return 0;
}
