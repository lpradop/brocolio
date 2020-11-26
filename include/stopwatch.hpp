#pragma once
#include <chrono>
class stopwatch {
public:
  stopwatch();
  ~stopwatch() = default;
  std::chrono::duration<long, std::ratio<1, 1'000'000'000>>
  elapsed_time() const;
  long milliseconds();

private:
  std::chrono::time_point<
      std::chrono::steady_clock,
      std::chrono::duration<long, std::ratio<1, 1'000'000'000>>>
      start_time_;
};
