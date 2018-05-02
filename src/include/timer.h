#ifndef _TIMER_H_
#define _TIMER_H_

#include <sys/timeb.h>
#include <utility.h>

namespace Nemesis {

namespace Time {

enum TimerState {
  STARTED,
  STOPPED,
};

class Timer {
private:
  TimerState state;
  unsigned long int start_time;
  unsigned long int milliseconds;

public:
  Timer();
  void start();
  void stop();
  unsigned long int get_time() const;
  void clear();
};
} // namespace Time
} // namespace Nemesis

#endif /* _TIMER_H_ */
