#pragma once
#include <type_traits>
namespace brocolio::container {

// unorede_pair by Brocolio de la CHUNSA
template <class T, class S> struct unordered_pair {
  T x{};
  S y{};

  bool operator==(unordered_pair const& other) {
    if constexpr (std::is_same<T, S>::value) {
      return (x == other.x || x == other.y) && (y == other.x || y == other.y);
    } else {
      return (x == other.x || y == other.y);
    }
  }

  operator unordered_pair<S, T>() const { return unordered_pair<S, T>{y, x}; }
  bool operator!=(unordered_pair const& other) { return !(*this == other); }
};

} // namespace brocolio::container
