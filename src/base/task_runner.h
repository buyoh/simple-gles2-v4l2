#ifndef SRC_BASE_TASK_RUNNER_H_
#define SRC_BASE_TASK_RUNNER_H_

#include <condition_variable>
#include <functional>
#include <map>
#include <memory>
#include <mutex>
#include <vector>

class TaskRunner {
 public:
  using RunnableTask = std::function<void()>;
  TaskRunner();
  ~TaskRunner();

  bool empty();
  void postTask(RunnableTask&& task);
  void postTaskDelayFor(RunnableTask&& task, long long delay_us);
  template <typename C, typename D>
  void postTaskDelayUntil(RunnableTask&& task,
                          std::chrono::time_point<C, D> p) {
    postTaskDelayUntilEpoch(std::move(task), p.time_since_epoch().count());
  }
  // return: is tasks non-empty?
  bool waitFor(long long timeout_us);
  void proceedAll();

 private:
  bool emptyNoLock() const { return tasks_.empty() && delay_tasks_.empty(); }
  void postTaskDelayUntilEpoch(RunnableTask&& task, uint64_t e);

  std::condition_variable cond_;
  std::mutex tasks_mutex_;
  std::multimap<uint64_t, RunnableTask> delay_tasks_;
  std::vector<RunnableTask> tasks_;

  std::shared_ptr<TaskRunner*> weak_ptr_factory_;
};

#endif  // SRC_BASE_TASK_RUNNER_H_