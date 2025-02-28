#ifndef TIMER_H
#define TIMER_H

#include <chrono>

class Timer {
public:
  Timer();

  void reset();
  void start();
  void stop();
  double getTime() const;

private:
  std::chrono::high_resolution_clock::time_point startTime;
  std::chrono::high_resolution_clock::time_point stopTime;
  bool running;
};

#endif
