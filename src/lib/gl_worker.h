#ifndef SRC_LIB_GL_WORKER_H_
#define SRC_LIB_GL_WORKER_H_

#include "lib/worker.h"

// A worker for opengl
class GlWorker : public Worker {
 public:
  GlWorker(TaskRunner& task_runner){};
  bool initialize(WorkerManager::Callback cb) final;
  void start() final;
  void stop() final;
  // TODO: bad impl? use builder?
  void addGlRenderer(GlRenderer&& renderer);

 protected:
  virtual bool glInitialize() = 0;
  virtual bool glWillRender() = 0;
  virtual bool glDoneRender() = 0;

 private:
  void startInternal();
  void mainloopTask();

  WorkerManager::Callback callback_;
  std::vector<GlRenderer> renderer_;
};

#endif  // SRC_LIB_GL_WORKER_H_