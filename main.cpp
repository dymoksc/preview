#include <iostream>
#include <chrono>

#include "threads/ThreadPool.h"

using namespace std::chrono;

int main() {
  auto start = high_resolution_clock::now();

  {
    threads::ThreadPool threadPool(10);
    for (int i = 0; i < 3; ++i) {
      threadPool.addTask([]() -> void {
        std::cout << std::this_thread::get_id() << " in\n";
        std::this_thread::sleep_for(1s);
        std::cout << std::this_thread::get_id() << " out\n";
      });
    }
    threadPool.waitForAllTasks();
  }

  auto finish = high_resolution_clock::now();
  std::cout << "Execution time: " << duration_cast<milliseconds>(finish - start).count() << "ms" << std::endl;

  return 0;
}
