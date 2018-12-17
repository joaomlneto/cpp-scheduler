
#include <iostream>
#include <thread>

#include <cpp-scheduler/Task.hpp>

namespace cpp_scheduler {

Task::Task(task_routine f) : _routine(f) {}

void Task::operator()() {
  _routine();
  std::unique_lock l(_lock);
  for (Task *t : _subscribers) {
    t->notifyTaskFinished(this);
  }
  l.unlock();
}

void Task::addDependency(Task *t) {
  _unfulfilledDependencies++;
  t->addSubscriber(this);
}

void Task::addSubscriber(Task *t) {
  std::scoped_lock l(_lock);
  _subscribers.push_back(t);
}

void Task::notifyTaskFinished(Task *t) {
  std::scoped_lock l(_lock);
  _unfulfilledDependencies--;
  if (isReady()) {
    _waitReady.notify_all();
  }
}

bool Task::isReady() {
  return _unfulfilledDependencies == 0;
}

void Task::waitReady() {
  std::unique_lock l(_lock);
  _waitReady.wait(l, [=](){
    return isReady();
  });
  l.unlock();
}

}  // namespace cpp_scheduler
