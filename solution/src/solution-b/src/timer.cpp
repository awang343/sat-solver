#include "timer.h"

// Constructor
Timer::Timer() : startTime(), stopTime(), running(false) {}

// Reset the timer
void Timer::reset() {
  startTime = std::chrono::high_resolution_clock::time_point();
  running = false;
}

// Start the timer
void Timer::start() {
  startTime = std::chrono::high_resolution_clock::now();
  running = true;
}

// Stop the timer
void Timer::stop() {
  if (running) {
    stopTime = std::chrono::high_resolution_clock::now();
    running = false;
  }
}

// Get elapsed time in seconds
double Timer::getTime() const {
  using namespace std::chrono;
  if (running) {
    return duration<double>(high_resolution_clock::now() - startTime).count();
  } else {
    return duration<double>(stopTime - startTime).count();
  }
}
