#include <timer.h>

Nemesis::Time::Timer::Timer() {
  clear();
  return;
}

void Nemesis::Time::Timer::clear() {
  state = STOPPED;
  start_time = 0;
  milliseconds = 0;
}

unsigned long int Nemesis::Time::Timer::get_time() const {
  if (state == STARTED)
    panic("get_time on running timer");
  return milliseconds;
}

void Nemesis::Time::Timer::start() {
  if (state == STARTED)
    panic("start on running timer");

  struct timeb sys_time;
  ftime(&sys_time);
  start_time = static_cast<unsigned long int>(sys_time.millitm) +
               static_cast<unsigned long int>(sys_time.time) * 1000UL;
  state = STARTED;
}

void Nemesis::Time::Timer::stop() {
  if (state == STOPPED)
    panic("stop on stopped timer");

  struct timeb sys_time;
  ftime(&sys_time);
  milliseconds += static_cast<unsigned long int>(sys_time.millitm) +
                  static_cast<unsigned long int>(sys_time.time) * 1000UL -
                  start_time;
  state = STOPPED;
}
