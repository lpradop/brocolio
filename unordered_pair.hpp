#pragma once
#include <type_traits>
namespace brocolio::container {

// unorede_pair by Brocolio de la CHUNSA
template <class T, class S> struct unordered_pair {
  T x{};
  S y{};
  unordered_pair(const T x, const S y) : x(x), y(y) {}

  bool operator==(const unordered_pair &other) {
    if constexpr (std::is_same<T, S>::value) {
      return (x == other.x or x == other.y) and (y == other.x or y == other.y);
    } else {
      return (x == other.x and y == other.y);
    }
  }

  operator unordered_pair<S, T>() const { return unordered_pair<S, T>{y, x}; }
  bool operator!=(const unordered_pair &other) { return not(*this == other); }
};

} // namespace brocolio::container
