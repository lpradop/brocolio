#pragma once
#include <chrono>
class stopwatch {
private:
  std::chrono::time_point<
      std::chrono::steady_clock,
      std::chrono::duration<long, std::ratio<1, 1000000000>>>
      start_time_;

public:
  stopwatch();
  ~stopwatch() = default;
  std::chrono::duration<long, std::ratio<1, 1000000000>> elapsed_time() const;
  long milliseconds();
};
