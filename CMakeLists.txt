cmake_minimum_required(VERSION 3.17)
project(preview)

set(CMAKE_CXX_STANDARD 17)

add_executable(preview main.cpp threads/ThreadPool.cc threads/ThreadPool.h web_server/RequestParser.cc web_server/RequestParser.h web_server/IRequestParser.h web_server/container/Request.h web_server/ParsingException.h web_server/RequestBuilder.cc web_server/RequestBuilder.h web_server/container/Header.h web_server/Server.h web_server/Server.cc web_server/container/Response.h web_server/IResponsePrinter.h web_server/ResponseBuilder.cc web_server/ResponseBuilder.h web_server/IRequestBuilder.h web_server/IResponseBuilder.h web_server/IUrlParser.h web_server/container/Protocol.h web_server/ResponsePrinter.cc web_server/HttpTransportEnumNames.cc web_server/HttpTransportEnumNames.h web_server/UrlParser.h web_server/UrlParser.cc system_wrappers/EpollWrapper.cc system_wrappers/EpollWrapper.h system_wrappers/SocketWrapper.cc system_wrappers/SocketWrapper.h system_wrappers/SocketConnectionWrapper.cc system_wrappers/SocketConnectionWrapper.h definitions.h)

include_directories(.)
include_directories(./libs)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries(preview PRIVATE Threads::Threads)

add_subdirectory(boost_tests)
