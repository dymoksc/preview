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

void ThreadPool::addTask(const std::function<void()>& task) {
  std::lock_guard<std::mutex> lockGuard(tasksMutex);
  tasks.push_back(task);
  std::cout << "Added task. Tasks count: " << tasks.size() << std::endl;
}

void ThreadPool::loop() {
  std::function<void()>* task = nullptr;

  while (true) {
    if (exitLoop) {
      break;
    }

    {
      std::lock_guard<std::mutex> lockGuard(tasksMutex);
      if (!tasks.empty()) {
        task = new std::function<void()>(tasks.back());
        tasks.pop_back();

        std::cout << std::this_thread::get_id() << " took task. " << tasks.size() << " tasks left" << std::endl;
      }
    }

    if (task != nullptr) {
      (*task)();
      delete task;
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
