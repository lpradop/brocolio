#pragma once
namespace brocolio::algorithm {
// generic function by Brocolio de la CHUNSA
template <class Container, class Predicate, class... Args>
typename Container::iterator search(Container const& container,
                                    Predicate const predicate,
                                    Args const... args) {
  for (auto it = container.begin(); it != container.end(); ++it) {
    if (predicate(*it, args...)) {
      return it;
    }
  }
  return container.end();
}

template <class Container, class Predicate, class... Args>
typename Container::iterator
search(Container const& container, typename Container::iterator iterator,
       Predicate const predicate, Args const... args) {
  for (; iterator != container.end(); ++iterator) {
    if (predicate(*iterator, args...)) {
      return iterator;
    }
  }
  return container.end();
}

template <class Iterator, class UnaryPredicate>
bool all_of(Iterator begin, Iterator end, UnaryPredicate unary_predicate) {
  for (; begin != end; ++begin) {
    if (not unary_predicate(*begin)) {
      return false;
    }
  }
  return true;
}
} // namespace brocolio::algorithm
