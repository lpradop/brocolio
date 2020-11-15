#pragma once
namespace brocolio::algorithm {

template <class Container, class Predicate, class... Args>
typename Container::iterator search(const Container &container,
                                    const Predicate predicate,
                                    const Args... args) {
  for (auto it = container.begin(); it != container.end(); ++it) {
    if (predicate(*it, args...)) {
      return it;
    }
  }
  return container.end();
}

template <class Container, class Predicate, class... Args>
typename Container::iterator
search(const Container &container, typename Container::iterator iterator,
       const Predicate predicate, const Args... args) {
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
