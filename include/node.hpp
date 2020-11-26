#pragma once
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

}; // namespace brocolio::container
