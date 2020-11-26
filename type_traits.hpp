#pragma once
#include <type_traits>

namespace brocolio::type_traits {

template <typename T> struct extract_parameter { using type = void; };

template <template <typename> typename X, typename T>
struct extract_parameter<X<T>> {
  using type = T;
};

template <typename T>
using extract_parameter_t = typename extract_parameter<T>::type;

} // namespace brocolio::type_traits
