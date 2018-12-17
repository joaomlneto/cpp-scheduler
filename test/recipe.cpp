
#include <cassert>
#include <iostream>
#include <chrono>
#include <cpp-scheduler/Task.hpp>
#include <cpp-scheduler/Scheduler.hpp>

int step = 0;

void go_shopping() {
  assert(++step == 1);
  std::cout << "went shopping for ingredients" << std::endl;
}

void go_home() {
  assert(++step == 2);
  std::cout << "went back home to start the cake" << std::endl;
}

void whip_whites() {
  step++;
  assert(step >= 3 && step <= 6);
  std::cout << "whip whites" << std::endl;
}

void choco_butter_mix() {
  step++;
  assert(step >= 3 && step <= 6);
  std::cout << "melting chocolate... takes a while" << std::endl;
  std::this_thread::sleep_for(std::chrono::milliseconds(600));
  std::cout << "mix chocolate and butter" << std::endl;
}

void yolk_sugar_flour_mix() {
  step++;
  assert(step >= 3 && step <= 6);
  std::cout << "mix yolks sugar and flour" << std::endl;
}

void preheat_oven() {
  step++;
  assert(step >= 3 && step <= 7);
  std::cout << "preheating oven..." << std::endl;
  std::this_thread::sleep_for(std::chrono::milliseconds(500));
  std::cout << "oven is now hot enough" << std::endl;
}

void mix_everything() {
  step++;
  assert(step >= 6 && step <= 7);
  std::cout << "mix whites, choco-butter and yolk-sugar-flour mix" << std::endl;
}

void put_in_oven() {
  assert(++step == 8);
  std::this_thread::sleep_for(std::chrono::milliseconds(200));
  std::cout << "put dough in the oven" << std::endl;
}

void eat() {
  assert(++step == 9);
  std::cout << "cake is ready! eating..." << std::endl;
}

int main() {
  cpp_scheduler::Scheduler s;

  // task declarations
  cpp_scheduler::Task *t_shopping = new cpp_scheduler::Task(go_shopping);
  cpp_scheduler::Task *t_home = new cpp_scheduler::Task(go_home);
  cpp_scheduler::Task *t_whites = new cpp_scheduler::Task(whip_whites);
  cpp_scheduler::Task *t_choco = new cpp_scheduler::Task(choco_butter_mix);
  cpp_scheduler::Task *t_yolks = new cpp_scheduler::Task(yolk_sugar_flour_mix);
  cpp_scheduler::Task *t_mix = new cpp_scheduler::Task(mix_everything);
  cpp_scheduler::Task *t_heat = new cpp_scheduler::Task(preheat_oven);
  cpp_scheduler::Task *t_bake = new cpp_scheduler::Task(put_in_oven);
  cpp_scheduler::Task *t_eat = new cpp_scheduler::Task(eat);

  // task dependencies
  t_home->addDependency(t_shopping);
  t_whites->addDependency(t_home);
  t_choco->addDependency(t_home);
  t_yolks->addDependency(t_home);
  t_heat->addDependency(t_home);
  t_mix->addDependency(t_whites);
  t_mix->addDependency(t_choco);
  t_mix->addDependency(t_yolks);
  t_bake->addDependency(t_heat);
  t_bake->addDependency(t_mix);
  t_eat->addDependency(t_bake);

  // schedule tasks (put shopping last, just to assert dependencies mgmt)
  s.schedule(t_home);
  s.schedule(t_whites);
  s.schedule(t_choco);
  s.schedule(t_yolks);
  s.schedule(t_mix);
  s.schedule(t_heat);
  s.schedule(t_bake);
  s.schedule(t_eat);
  s.schedule(t_shopping);

  // wait for everything to finish
  s.waitFinish();
  std::cout << "yum!" << std::endl;
  return 0;
}
