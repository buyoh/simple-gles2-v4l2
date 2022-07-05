
#include "base/task_runner.h"
#include <chrono>
#include <cstdint>
#include <iostream>
#include <memory>
#include <thread>

TaskRunner::Client::Client(std::weak_ptr<TaskRunner> task_runner)
    : task_runner_(std::move(task_runner)) {
  //
}

void TaskRunner::Client::postTask(RunnableTask&& task) {
  // check that task_runner stil alive
  auto tr = task_runner_.lock();
  if (!tr)
    return;
  tr->onPostTask(std::move(task));
}

void TaskRunner::Client::postTaskDelayFor(RunnableTask&& task,
                                          long long delay_us) {
  postTaskDelayUntil(std::move(task), std::chrono::steady_clock::now() +
                                          std::chrono::microseconds(delay_us));
}

void TaskRunner::Client::postTaskDelayUntilEpoch(RunnableTask&& task,
                                                 uint64_t e) {
  // check that task_runner stil alive
  auto tr = task_runner_.lock();
  if (!tr)
    return;
  tr->onPostTaskDelayUntilEpoch(std::move(task), e);
}

TaskRunner::TaskRunner()
    : weak_ptr_factory_(std::make_shared<TaskRunner*>(this)) {}

TaskRunner::~TaskRunner() {
  //
  std::lock_guard<std::mutex> lock(tasks_mutex_);
}

// static
std::shared_ptr<TaskRunner>& TaskRunner::getThreadLocalShared() {
  thread_local std::shared_ptr<TaskRunner> instance(new TaskRunner());
  return instance;
}

// static
TaskRunner& TaskRunner::getThreadLocal() {
  return *getThreadLocalShared();
}

// static
TaskRunner::Client TaskRunner::getThreadLocalClient() {
  return TaskRunner::Client(std::weak_ptr<TaskRunner>(getThreadLocalShared()));
}

// std::weak_ptr<TaskRunner*> TaskRunner::getWeakPtr() {
//   return std::weak_ptr<TaskRunner*>(weak_ptr_factory_);
// }

bool TaskRunner::empty() {
  std::lock_guard<std::mutex> lock(tasks_mutex_);
  return emptyNoLock();
}

bool TaskRunner::waitFor(long long timeout_us) {
  std::unique_lock<std::mutex> lock(tasks_mutex_);
  if (!emptyNoLock())
    return true;
  cond_.wait_for(lock, std::chrono::microseconds(timeout_us),
                 [&] { return !emptyNoLock(); });
  return !emptyNoLock();
}

void TaskRunner::proceedAll() {
  std::vector<RunnableTask> tasks;
  do {
    uint64_t now_epoch =
        std::chrono::steady_clock::now().time_since_epoch().count();
    {
      std::lock_guard<std::mutex> lock(tasks_mutex_);
      tasks.swap(tasks_);
      // queue delay_task
      for (auto it = delay_tasks_.begin(); it != delay_tasks_.end();) {
        if (it->first <= now_epoch) {
          tasks.push_back(std::move(it->second));
          it = delay_tasks_.erase(it);
          continue;
        }
        ++it;
      }
    }

    if (tasks.empty())
      break;

    // Proceed all task
    for (auto& t : tasks) {
      t();
    }

    tasks.clear();

  } while (true);
}

void TaskRunner::onPostTask(RunnableTask&& task) {
  std::lock_guard<std::mutex> lock(tasks_mutex_);
  tasks_.push_back(std::move(task));
  cond_.notify_one();
}

void TaskRunner::onPostTaskDelayUntilEpoch(RunnableTask&& task, uint64_t e) {
  std::lock_guard<std::mutex> lock(tasks_mutex_);
  delay_tasks_.emplace(std::move(e), std::move(task));
  // no cond_.notify_one();
}