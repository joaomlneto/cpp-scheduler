
#ifndef INCLUDE_CPP_SCHEDULER_SCHEDULER_HPP_
#define INCLUDE_CPP_SCHEDULER_SCHEDULER_HPP_

/**
5.1) Can you please code a small scheduler application in C++, with
     a makefile, which implements the following features:
     a) A task should be added as a functor.
     b) Should be possible to schedule tasks at:
        1) fixed rate;
        2) given delay.
     c) Set dependencies between tasks;
*/

#include <vector>

#include <cpp-scheduler/Task.hpp>
#include <chrono>
#include <thread>

namespace cpp_scheduler {

class Scheduler {
 private:
  unsigned int            _pendingTasks = 0;
  std::vector<Task*>      _tasks;
  std::mutex              _lock;
  std::condition_variable _waitFinish;

  void notifyTaskCompletion(Task *t);

 public:
  void schedule(Task*);
  void scheduleAfter(Task *t, unsigned int ms);
  void schedulePeriodic(Task *t, unsigned int ms, unsigned int limit);
  bool hasFinished();
  void waitFinish();
};

}  // namespace cpp_scheduler

#endif  // INCLUDE_CPP_SCHEDULER_SCHEDULER_HPP_
