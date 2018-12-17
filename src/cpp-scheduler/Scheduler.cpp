
#include <iostream>
#include <chrono>

#include <cpp-scheduler/Scheduler.hpp>

namespace cpp_scheduler {

void Scheduler::notifyTaskCompletion(Task *t) {
  std::scoped_lock l(_lock);
  _pendingTasks--;
  if (hasFinished()) {
    _waitFinish.notify_all();
  }
}

void Scheduler::schedule(Task *t) {
  std::unique_lock l(_lock);
  _pendingTasks++;
  l.unlock();
  std::thread taskThread([&, t]() {
    t->waitReady();
    (*t)();
    notifyTaskCompletion(t);
  });
  taskThread.detach();
}

void Scheduler::scheduleAfter(Task *t, unsigned int ms) {
  std::unique_lock l(_lock);
  _pendingTasks++;
  l.unlock();
  std::thread taskThread([&, t, ms]() {
    std::this_thread::sleep_for(std::chrono::milliseconds(ms));
    t->waitReady();
    (*t)();
    notifyTaskCompletion(t);
  });
  taskThread.detach();
}

void Scheduler::schedulePeriodic(Task *t, unsigned int ms, unsigned int limit) {
  std::unique_lock l(_lock);
  _pendingTasks++;
  l.unlock();
  std::thread taskThread([&, t, ms, limit]() {
    unsigned int remaining = limit;
    t->waitReady();
    while (remaining--) {
      std::this_thread::sleep_for(std::chrono::milliseconds(ms));
      (*t)();
    }
    notifyTaskCompletion(t);
  });
  taskThread.detach();
}

bool Scheduler::hasFinished() {
  return this->_pendingTasks == 0;
}

void Scheduler::waitFinish() {
  std::unique_lock l(_lock);
  _waitFinish.wait(l, [=](){
    return hasFinished();
  });
  l.unlock();
}

}  // namespace cpp_scheduler
