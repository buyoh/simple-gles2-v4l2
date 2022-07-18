#ifndef SRC_LIB_WORKER_MANAGER_H_
#define SRC_LIB_WORKER_MANAGER_H_

#include <memory>
#include <vector>

#include "lib/worker.h"

// WorkerManager is used for managing main thread.
// - Workers run asynchronous / on another thread.
// - Workers run on main thread via WorkerManager::Callback as it need.
// TODO: rename to WorkerRunner?
class WorkerManager {
 public:
  class Callback {
   public:
    Callback();
    // should be run on main thread!
    void stopAllWorker();

   private:
    friend WorkerManager;
    Callback(std::weak_ptr<WorkerManager*> manager);

    std::weak_ptr<WorkerManager*> manager_;
  };
  WorkerManager();
  // TODO: bad impl? use builder?
  void addWorker(std::unique_ptr<Worker> worker);
  bool startAllWorker();

 private:
  void onStopAllworker();

  std::vector<std::unique_ptr<Worker>> workers_;
  std::shared_ptr<WorkerManager*> weak_ptr_factory_;
};

#endif  // SRC_LIB_WORKER_MANAGER_H_