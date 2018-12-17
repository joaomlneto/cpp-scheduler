
#include <cassert>
#include <iostream>
#include <chrono>
#include <cpp-scheduler/Task.hpp>
#include <cpp-scheduler/Scheduler.hpp>

int step = 0;

void t1_func() {
  assert(++step == 2);
  std::cout << "two" << std::endl;
}
void t2_func() {
  std::this_thread::sleep_for(std::chrono::milliseconds(200));
  assert(++step == 1);
  std::cout << "one" << std::endl;
}

int main() {
  cpp_scheduler::Scheduler s;
  cpp_scheduler::Task *t1 = new cpp_scheduler::Task(t1_func);
  cpp_scheduler::Task *t2 = new cpp_scheduler::Task(t2_func);
  t1->addDependency(t2);
  s.schedule(t1);
  s.schedule(t2);
  s.waitFinish();
  return 0;
}
