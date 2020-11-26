#include "stopwatch.hpp"
stopwatch::stopwatch() : start_time_(std::chrono::steady_clock::now()) {}

std::chrono::duration<long, std::ratio<1, 1'000'000'000>>
stopwatch::elapsed_time() const {
  std::chrono::time_point<
      std::chrono::steady_clock,
      std::chrono::duration<long, std::ratio<1, 1'000'000'000>>>
      now = std::chrono::steady_clock::now();
  return now - start_time_;
}

long stopwatch::milliseconds() {
  return std::chrono::duration_cast<std::chrono::milliseconds>(
             this->elapsed_time())
      .count();
}
