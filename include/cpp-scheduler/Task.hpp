
#ifndef INCLUDE_CPP_SCHEDULER_TASK_HPP_
#define INCLUDE_CPP_SCHEDULER_TASK_HPP_

#include <string>
#include <vector>
#include <mutex>
#include <condition_variable>

namespace cpp_scheduler {

// FIXME(joaomlneto): add function signature as template
typedef void (*task_routine)();

class Task {
 private:
  // the code to be executed when the task runs
  task_routine            _routine;
  // number of remaining dependencies to be fulfilled
  unsigned int            _unfulfilledDependencies = 0;
  // task dependents (to be notified when this task completes)
  std::vector<Task*>      _subscribers;

  std::mutex              _lock;
  // conditional variable to wait until this task is ready
  std::condition_variable _waitReady;

  // notify this task that one of its dependencies finished
  void notifyTaskFinished(Task *t);

 public:
  // create a task with a given routine
  // TODO(joaomlneto): we should be able to accept lambdas as well!
  explicit Task(task_routine);

  // run the task and afterwards notify dependents of completion
  void operator()();

  // add a task dependency
  void addDependency(Task *t);

  // add a subscribed (to be notified when finished)
  void addSubscriber(Task *t);

  // check if task no unfulfilled dependencies
  bool isReady();

  // block until this task is ready to execute
  void waitReady();
};

}  // namespace cpp_scheduler

#endif  // INCLUDE_CPP_SCHEDULER_TASK_HPP_
