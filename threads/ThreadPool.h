#ifndef PREVIEW_THREADPOOL_H
#define PREVIEW_THREADPOOL_H

#include <functional>
#include <thread>
#include <atomic>
#include <mutex>
#include <future>
#include <queue>

namespace threads {

class ThreadPool {
private:
  std::vector<std::thread> threads;
  std::atomic<bool> exitLoop = false;
  std::queue<std::function<void()>> tasks;
  mutable std::mutex tasksMutex;

public:
  explicit ThreadPool(size_t threadsCount);
  ~ThreadPool();

  template <class Func, class... Arguments, typename ReturnType = typename std::result_of_t<Func(Arguments...)>>
  std::future<ReturnType> addTask(Func&& func, Arguments&&... arguments) {
    auto task = std::make_shared<std::packaged_task<ReturnType()>>(std::bind(std::forward<Func>(func), std::forward<Arguments>(arguments)...));
    {
      std::lock_guard<std::mutex> guard(tasksMutex);
      tasks.emplace([task]() -> void { (*task)(); });
    }

    return task->get_future();
  }

  void loop();
  void waitForAllTasks() const;
};

}


#endif //PREVIEW_THREADPOOL_H
