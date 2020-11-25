#pragma once
#include "binary_search_tree.hpp"
#include <iostream>
namespace brocolio::container {
template <class KeyType, class DataType, class WeightType>
class binary_weighted_tree {
public:
  binary_weighted_tree() = default;
  binary_weighted_tree(binary_weighted_tree const&); // TODO
  binary_weighted_tree(binary_weighted_tree&&);
  binary_weighted_tree(KeyType root_key, DataType const data);
  // binary_weighted_tree(DataType const data);
  ~binary_weighted_tree(); // TODO
  void insert(KeyType const key, DataType const data, WeightType const weight,
              KeyType const parent_key, bool left_child = true);
  bool remove(DataType const data);
  bool remove(KeyType const key);
  // bool search(KeyType const key) const;
  std::size_t size() const { return size_; };
  bool empty() const { return size_ == 0; };

  void
  print(transversal_method const method = transversal_method::in_order) const;

private:
  struct node;
  node* root_{nullptr};
  std::size_t size_{0};
  // bool const auto_generate_keys{false}; //TODO

  bool remove_node(node* const given_node);
};

template <class KeyType, class DataType, class WeightType>
struct binary_weighted_tree<KeyType, DataType, WeightType>::node {
  KeyType key{};
  DataType data{};
  WeightType weight{};
  node* left{nullptr};
  node* right{nullptr};
  node* parent{nullptr};
};

template <class KeyType, class DataType, class WeightType>
binary_weighted_tree<KeyType, DataType, WeightType>::binary_weighted_tree(
    binary_weighted_tree const& other)
    : size_(other.size_) {}

template <class KeyType, class DataType, class WeightType>
binary_weighted_tree<KeyType, DataType, WeightType>::binary_weighted_tree(
    binary_weighted_tree&& other)
    : size_(other.size_), root_(other.root_) {
  other.root_ == nullptr;
  other.size_ == 0;
}

template <class KeyType, class DataType, class WeightType>
binary_weighted_tree<KeyType, DataType, WeightType>::binary_weighted_tree(
    KeyType root_key, DataType const data)
    : root_(new node{root_key, data, WeightType{}, nullptr, nullptr, nullptr}) {
}

// template <class KeyType, class DataType, class WeightType>
// binary_weighted_tree<KeyType, DataType, WeightType>::binary_weighted_tree(
//     DataType const data)
//     : root_(new node{KeyType{}, data, WeightType{}, nullptr, nullptr,
//     nullptr}),
//       auto_generate_keys(true) {}

template <class KeyType, class DataType, class WeightType>
binary_weighted_tree<KeyType, DataType, WeightType>::~binary_weighted_tree() {}

template <class KeyType, class DataType, class WeightType>
void binary_weighted_tree<KeyType, DataType, WeightType>::insert(
    KeyType const key, DataType const data, WeightType const weight,
    KeyType const parent_key, bool left_child) {}

template <class KeyType, class DataType, class WeightType>
bool binary_weighted_tree<KeyType, DataType, WeightType>::remove(
    DataType data) {}
}; // namespace brocolio::container
