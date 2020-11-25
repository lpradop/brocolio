#pragma once
#include "node.hpp"
#include <iostream>
#include <type_traits>
namespace brocolio::container {
// if simple_node
template <typename NodeType, typename Enable = void> class base_linked_list {};

// if doubly_node
template <typename NodeType>
class base_linked_list<
    NodeType, std::enable_if_t<std::is_same_v<
                  NodeType, doubly_node<extract_parameter_t<NodeType>>>>> {
protected:
  NodeType* tail_{nullptr};
};

template <Node NodeType> class linked_list : base_linked_list<NodeType> {
public:
  class iterator;
  linked_list() = default;
  linked_list(linked_list const&);
  linked_list(linked_list&&);
  ~linked_list();
  void insert(extract_parameter_t<NodeType> const data,
              std::size_t const position);

  void remove(std::size_t const position);
  iterator begin() const;
  iterator end() const;
  void foo() {
    base_linked_list<doubly_node<extract_parameter_t<NodeType>>>::tail_ =
        nullptr;
  }
  std::size_t size() const;
  void clear();
  void print() const;
  bool empty() const { return size_ == 0; }

private:
  std::size_t size_{0};
  NodeType* head_{nullptr};
};

// template <class DataType> class linked_list<DataType>::iterator {
// public:
// iterator() = default;
// iterator(node* it_node) : it_node_(it_node){};
// ~iterator() = default;
// iterator& operator++() {
// if (it_node_ != nullptr) {
// it_node_ = it_node_->next;
// }
// return *this;
// };
// iterator operator++(int) {
// if (it_node_ != nullptr) {
// iterator tmp{*this};
// operator++();
// return tmp;
// } else {
// return *this;
// }
// }
// DataType& operator*() const { return (this->it_node_)->data; }
// bool operator==(iterator const& other) const {
// return (this->it_node_ == other.it_node_);
// }
// bool operator!=(iterator const& other) const { return not(*this == other); }
//
// private:
// node* it_node_{nullptr};
// };

template <Node NodeType>
void linked_list<NodeType>::insert(extract_parameter_t<NodeType> const data,
                                   std::size_t const position) {
  if constexpr (std::is_same_v<NodeType,
                               simple_node<extract_parameter_t<NodeType>>>) {
    if (position < size_) {
      if (position == 0) {
        head_ = new NodeType{data, head_};
      } else {
        NodeType* tmp{head_};
        for (std::size_t i{0}; i < position - 1; ++i) {
          tmp = tmp->next;
        }
        tmp->next = new NodeType{data, tmp->next};
      }
    } else {
      if (size_ == 0) {
        head_ = new NodeType{data, nullptr};
      } else {
        NodeType* tmp{head_};
        for (; tmp->next != nullptr; tmp = tmp->next)
          ;
        tmp->next = new NodeType{data, nullptr};
      }
    }
  }
  ++size_;
}

template <Node NodeType>
void linked_list<NodeType>::remove(std::size_t const position) {
  if constexpr (std::is_same_v<NodeType,
                               simple_node<extract_parameter_t<NodeType>>>) {
    // simple linked list
    if (position < size_) {
      NodeType* previous_node{head_};
      NodeType* current_node{previous_node->next};

      if (position == 0) {
        head_ = current_node;
        delete previous_node;
      } else {

        for (std::size_t i{0}; i < position - 1; ++i) {
          previous_node = current_node;
          current_node = current_node->next;
        }

        previous_node->next = current_node->next;
        delete current_node;
      }
    }
  }

  if (size_ != 0) {
    --size_;
  }
}

template <Node NodeType>
linked_list<NodeType>::linked_list(linked_list const& other)
    : size_(other.size_) {
  if constexpr (std::is_same_v<NodeType,
                               simple_node<extract_parameter_t<NodeType>>>) {
    // simple linked list
    if (size_ != 0) {
      head_ = new NodeType{other.head_->data, nullptr};
      NodeType* other_node{other.head_->next};
      NodeType* this_node{head_};

      while (other_node != nullptr) {
        this_node->next = new NodeType{other_node->data, nullptr};
        this_node = this_node->next;
        other_node = other_node->next;
      }
    }
  } else {
    // doubly linked list
  }
}

template <Node NodeType>
linked_list<NodeType>::linked_list(linked_list&& other)
    : head_(other.head_), size_(other.size_) {
  other.head_ = nullptr;
  other.size_ = 0;
}

template <Node NodeType> linked_list<NodeType>::~linked_list() { clear(); }

template <Node NodeType>
inline std::size_t linked_list<NodeType>::size() const {
  return size_;
}

template <Node NodeType> void linked_list<NodeType>::clear() {
  for (NodeType* tmp{nullptr}; head_ != nullptr; head_ = tmp) {
    tmp = head_->next;
    delete head_;
  }
  size_ = 0;
}

// template <class NodType>
// typename linked_list<NodType>::iterator linked_list<NodType>::begin() const
// {
//   return iterator{head_};
// }

// template <class NodType>
// typename linked_list<NodType>::iterator linked_list<NodType>::end() const {
//   return iterator{nullptr};
// }

template <Node NodeType> void linked_list<NodeType>::print() const {
  for (NodeType const* tmp{head_}; tmp != nullptr; tmp = tmp->next) {
    std::cout << tmp->data << " -> ";
  }
  std::cout << "NULL" << std::endl;
}

} // namespace brocolio::container
