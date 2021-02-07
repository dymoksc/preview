#ifndef PREVIEW_THREADPOOL_H
#define PREVIEW_THREADPOOL_H

#include <functional>
#include <thread>
#include <atomic>
#include <mutex>

namespace threads {

class ThreadPool {
private:
  std::vector<std::thread> threads;
  std::atomic<bool> exitLoop = false;
  std::vector<std::function<void()>> tasks;
  mutable std::mutex tasksMutex;

public:
  explicit ThreadPool(size_t threadsCount);
  ~ThreadPool();
  void addTask(const std::function<void()>& task);
  void loop();
  void waitForAllTasks() const;
};

}


#endif //PREVIEW_THREADPOOL_H
