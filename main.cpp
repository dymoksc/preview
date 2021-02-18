#include <iostream>

#include <arpa/inet.h>
#include <csignal>
#include <sys/epoll.h>
#include <libs/di/include/boost/di.hpp>

#include "system_wrappers/EpollWrapper.h"
#include "system_wrappers/SocketConnectionWrapper.h"
#include "system_wrappers/SocketWrapper.h"
#include "threads/ThreadPool.h"
#include "web_server/IRequestParser.h"
#include "web_server/IResponsePrinter.h"
#include "web_server/IUrlParser.h"
#include "web_server/RequestParser.h"
#include "web_server/ResponsePrinter.h"
#include "web_server/Server.h"
#include "web_server/UrlParser.h"

#define THREADS_COUNT 3

using std::cout;
using std::cerr;
using std::endl;

volatile bool exitFlag = false;

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
  system_wrappers::SocketWrapper socketWrapper;

  cerr << "Setting up epoll..." << endl;
  system_wrappers::EpollWrapper epollWrapper(socketWrapper.getFd());

  cerr << "Entering the main loop..." << endl;
  std::signal(SIGTERM, [](int) { exitFlag = true; });
  try {
    while (!exitFlag) {
      auto epollEventsIterators = epollWrapper.wait();
      std::for_each(
          epollEventsIterators.first,
          epollEventsIterators.second,
          [&socketWrapper, &injector](const epoll_event& epollEvent) -> void {
            cerr << "Event on fd #" << epollEvent.data.fd << endl;
            if (epollEvent.events & EPOLLIN) {
              system_wrappers::SocketConnectionWrapper socketConnectionWrapper = socketWrapper.accept();
              cout << "Client connected " << inet_ntoa(socketConnectionWrapper.getClientAddr().sin_addr) << endl;
              auto server = injector.create<std::shared_ptr<web_server::Server>>();
              socketConnectionWrapper.send(server->getResponse(socketConnectionWrapper.receive()));
            }
          }
      );
    }
  } catch (const std::runtime_error& e) {
    if (!exitFlag) {
      throw e;
    }
  }

  cerr << "Exiting..." << endl;

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
