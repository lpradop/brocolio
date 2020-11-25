#pragma once
namespace brocolio {
namespace container {

// OrderedPair by Brocolio de la CHUNSA
template <class T, class S> struct ordered_pair {
  T x{};
  S y{};
  bool operator==(ordered_pair<T, S> const& other) const {
    return (x == other.x and y == other.y);
  };

  bool operator!=(ordered_pair<T, S> const other) const {
    return !(*this == other);
  }
};

} // namespace container
} // namespace brocolio
