#pragma once
namespace brocolio {
namespace container {

// OrderedPair by Brocolio de la CHUNSA
template <class T, class S> struct ordered_pair {
  T x{};
  S y{};
  ordered_pair() = default;
  ordered_pair(const ordered_pair &) = default;
  ~ordered_pair() = default;
  ordered_pair(const T x, const S y) : x(x), y(y) {}

  bool operator==(const ordered_pair<T, S> &other) {
    return (x == other.x and y == other.y);
  };

  bool operator!=(const ordered_pair<T, S> other) {
    return not(*this == other);
  }
};

} // namespace container
} // namespace brocolio
