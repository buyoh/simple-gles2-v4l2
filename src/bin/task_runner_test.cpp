// sample code

#include "base/task_runner.h"
#include <unistd.h>
#include <thread>
#include "base/logging.h"

int main() {
  bool running = true;

  VLOG(0) << std::this_thread::get_id();
  // create task 1
  std::thread t1(
      [](TaskRunner::Client task_runner_client) {
        VLOG(0) << std::this_thread::get_id();
        sleep(1);
        task_runner_client.postTask([]() {
          //
          VLOG(0) << std::this_thread::get_id();
        });
        sleep(2);
        task_runner_client.postTask([]() {
          //
          VLOG(0) << std::this_thread::get_id();
        });
      },
      TaskRunner::getThreadLocalClient());

  // create task 2
  TaskRunner::getThreadLocalClient().postTaskDelayFor(
      []() {
        //
        VLOG(0) << std::this_thread::get_id();
      },
      1000 * 1000 * 2);

  // main loop
  TaskRunner& task_runner = TaskRunner::getThreadLocal();

  while (running) {
    bool has_tasks = task_runner.waitFor(1000);
    if (has_tasks)
      task_runner.proceedAll();
  }
}