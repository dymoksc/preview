#include "ThreadPool.h"

#include <iostream>

namespace threads {

ThreadPool::ThreadPool(size_t threadsCount) {
  threads.reserve(threadsCount);
  for (int i = 0; i < threadsCount; ++i) {
    threads.emplace_back([this]{ loop(); });
  }
}

ThreadPool::~ThreadPool() {
  exitLoop = true;
  for (std::thread& thread : threads) {
    thread.join();
  }
}

void ThreadPool::loop() {
  std::function<void()> task = nullptr;

  while (true) {
    if (exitLoop) {
      break;
    }

    {
      std::lock_guard<std::mutex> lockGuard(tasksMutex);
      if (!tasks.empty()) {
        task = tasks.front();
        tasks.pop();
      }
    }

    if (task) {
      task();
      task = nullptr;
    }
  }

}

void ThreadPool::waitForAllTasks() const {
  while (true) {
    std::lock_guard<std::mutex> lockGuard(tasksMutex);
    if (tasks.empty()) {
      return;
    }
  }
}

}
