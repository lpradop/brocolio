#pragma once
#include <type_traits>
namespace brocolio::container {

template <typename T> struct extract_parameter { using type = void; };

template <template <typename> typename X, typename T>
struct extract_parameter<X<T>> {
  using type = T;
};

template <typename T>
using extract_parameter_t = typename extract_parameter<T>::type;

template <typename DataType> struct simple_node {
  DataType data{};
  simple_node* next{nullptr};
};

template <typename DataType> struct doubly_node {
  DataType data{};
  doubly_node* previous{nullptr};
  doubly_node* next{nullptr};
};

template <typename NodeType>
concept Node =
    std::is_same_v<NodeType, doubly_node<extract_parameter_t<NodeType>>> ||
    std::is_same_v<NodeType, simple_node<extract_parameter_t<NodeType>>>;

}; // namespace brocolio::container
