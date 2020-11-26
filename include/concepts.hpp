#pragma once
#include "type_traits.hpp"
#include <concepts>
namespace brocolio::concepts {

template <typename DataType> struct simple_node;
template <typename DataType> struct doubly_node;
template <typename DataType> struct circular_node;

template <typename NodeType>
concept node =
    std::is_same_v<NodeType,
                   simple_node<type_traits::extract_parameter_t<NodeType>>> ||

    std::is_same_v<NodeType,
                   doubly_node<type_traits::extract_parameter_t<NodeType>>> ||

    std::is_same_v<NodeType,
                   circular_node<type_traits::extract_parameter_t<NodeType>>>;

template <typename T>
concept numeric = std::integral<T> || std::floating_point<T>;
} // namespace brocolio::concepts
