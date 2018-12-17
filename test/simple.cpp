
#include <iostream>
#include <cpp-scheduler/Task.hpp>
#include <cpp-scheduler/Scheduler.hpp>

void t1_func() {
  std::cout << "one second has passed" << std::endl;
}
void t2_func() {
  std::cout << "this should be the first task" << std::endl;
}
void t_annoying() {
  std::cout << "*knock knock* penny!" << std::endl;
}

int main() {
  cpp_scheduler::Scheduler s;
  cpp_scheduler::Task t1(t1_func);
  cpp_scheduler::Task t2(t2_func);
  cpp_scheduler::Task t3(t_annoying);
  s.scheduleAfter(&t1, 800);
  s.schedule(&t2);
  s.schedulePeriodic(&t3, 400, 3);
  s.waitFinish();
  return 0;
}
