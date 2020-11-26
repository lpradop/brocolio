#pragma once
#include "type_traits.hpp"
#include <type_traits>
namespace brocolio::container {

template <typename DataType> struct simple_node {
  DataType data{};
  simple_node* next{nullptr};
};

template <typename DataType> struct doubly_node {
  DataType data{};
  doubly_node* previous{nullptr};
  doubly_node* next{nullptr};
};

template <typename DataType> struct circular_node {
  DataType data{};
  circular_node* previous{nullptr};
  circular_node* next{nullptr};
};

template <typename NodeType>
concept Node =
    std::is_same_v<NodeType,
                   simple_node<type_traits::extract_parameter_t<NodeType>>> ||

    std::is_same_v<NodeType,
                   doubly_node<type_traits::extract_parameter_t<NodeType>>> ||

    std::is_same_v<NodeType,
                   circular_node<type_traits::extract_parameter_t<NodeType>>>;

}; // namespace brocolio::container
