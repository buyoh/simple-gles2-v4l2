// sample code

#include "base/task_runner.h"
#include <unistd.h>
#include <thread>
#include "base/logging.h"

int main() {
  TaskRunner task_runner;
  bool running = true;

  VLOG(0) << std::this_thread::get_id();
  std::thread t1(
      [](TaskRunner* task_runner, bool* running) {
        VLOG(0) << std::this_thread::get_id();
        sleep(1);
        task_runner->postTask([]() {
          //
          VLOG(0) << std::this_thread::get_id();
        });
        sleep(2);
        task_runner->postTask([]() {
          //
          VLOG(0) << std::this_thread::get_id();
        });
      },
      &task_runner, &running);

  task_runner.postTaskDelayFor(
      []() {
        //
        VLOG(0) << std::this_thread::get_id();
      },
      1000 * 1000 * 2);

  while (running) {
    bool has_tasks = task_runner.waitFor(1000);
    if (has_tasks)
      task_runner.proceedAll();
  }
}