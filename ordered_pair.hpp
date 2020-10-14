#pragma once
namespace brocolio {
namespace container {

// OrderedPair by Brocolio de la CHUNSA
template <class T, class S> struct OrderedPair {
  T x{};
  S y{};
  OrderedPair() = default;
  ~OrderedPair() = default;
  OrderedPair(const T x, const S y);
  bool operator==(const OrderedPair<T, S> other) {
    return (x == other.x and y == other.y);
  };
  bool operator!=(const OrderedPair<T, S> other) { return not(*this == other); }
};

template <class T, class S>
OrderedPair<T, S>::OrderedPair(const T x, const S y) : x(x), y(y) {}
} // namespace container
} // namespace brocolio
