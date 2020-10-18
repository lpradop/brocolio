#pragma once
namespace brocolio {
namespace algorithm {

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
} // namespace algorithm
} // namespace brocolio
