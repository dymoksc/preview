cmake_minimum_required(VERSION 3.17)
project(preview)

set(CMAKE_CXX_STANDARD 17)

add_executable(preview main.cpp threads/ThreadPool.cc threads/ThreadPool.h)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries(preview PRIVATE Threads::Threads)
