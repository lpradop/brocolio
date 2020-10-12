#pragma once
template <class T, class S> struct OrderedPair {
  T x{};
  S y{};
  OrderedPair();
  OrderedPair(const T x, const S y);
  bool operator==(const OrderedPair<T, S> other) {
    return (x == other.x and y == other.y);
  };
  bool operator!=(const OrderedPair<T, S> other) { return not(*this == other); }
  bool Contains(const T element);
};

template <class T, class S> bool OrderedPair<T, S>::Contains(const T element) {
  return (element == x or element == y);
}

template <class T, class S>
OrderedPair<T, S>::OrderedPair(const T x, const S y) : x(x), y(y) {}
