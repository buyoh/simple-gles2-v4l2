#ifndef SRC_LIB_WORKER_H_
#define SRC_LIB_WORKER_H_

#include "lib/worker_manager.h"

// A task run asynchronous / on another thread.
class Worker {
 public:
  // TODO:
  virtual bool initialize(WorkerManager::Callback cb) = 0;
  // Start worker.
  virtual void start() = 0;
  // The work will stop
  virtual void stop() = 0;
};
#endif  // SRC_LIB_WORKER_H_