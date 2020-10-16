#pragma once

#include "ordered_pair.hpp"
#include <iostream>
#include <utility>
namespace brocolio {
namespace container {

template <class DataType> class simple_linked_list {
private:
  struct node;
  node *head_{nullptr};
  std::size_t size_{0};

public:
  class iterator;
  simple_linked_list() = default;
  ~simple_linked_list();
  simple_linked_list(const simple_linked_list &);
  void insert(const DataType data, const std::size_t position = 0);
  void remove(const std::size_t position = 0);
  iterator begin() const;
  iterator end() const;
  std::size_t size() const;
  void clear();
  void print() const;
};

template <class DataType> struct simple_linked_list<DataType>::node {
  DataType data{};
  node *next{nullptr};
  node() = default;
  node(DataType data, node *next) : data(data), next(next) {}
  ~node() = default;
};

template <class DataType> class simple_linked_list<DataType>::iterator {
private:
  node *node_{nullptr};
  template <class T> friend class SimpleLinkedList;

public:
  iterator() = default;
  iterator(const iterator &) = default;
  iterator(node *node) : node_(node){};
  ~iterator() = default;
  iterator &operator++() {
    if (node_ != nullptr) {
      node_ = node_->next;
    }
    return *this;
  };
  iterator operator++(int) {
    if (node_ != nullptr) {
      iterator tmp{*this};
      operator++();
      return tmp;
    } else {
      return *this;
    }
  }
  DataType &operator*() const { return (this->node_)->data; }
  bool operator==(const iterator &obj) const {
    return (this->node_ == obj.node_);
  }
  bool operator!=(const iterator &obj) const { return not(*this == obj); }
};

template <class DataType>
void simple_linked_list<DataType>::insert(const DataType data,
                                          const std::size_t position) {
  if (position < size_) {
    if (position == 0) {
      head_ = new node(data, head_);
    } else {
      node *tmp = head_;

      for (unsigned int i = 0; i < position - 1; ++i) {
        tmp = tmp->next;
      }
      tmp->next = new node(data, tmp->next);
    }
  } else {
    if (size_ == 0) {
      head_ = new node(data, nullptr);
    } else {
      node *tmp = head_;
      while (tmp != nullptr) {
        if (tmp->next == nullptr)
          break;
        tmp = tmp->next;
      }
      tmp->next = new node(data, nullptr);
    }
  }
  ++size_;
}

template <class DataType>
void simple_linked_list<DataType>::remove(std::size_t position) {
  if (position < size_) {
    node *previous_node = head_;
    node *current_node = previous_node->next;

    if (position == 0) {
      head_ = current_node;
      delete previous_node;
    } else {

      for (unsigned int i = 0; i < position - 1; ++i) {
        previous_node = current_node;
        current_node = current_node->next;
      }

      previous_node->next = current_node->next;
      delete current_node;
    }
    --size_;
  }
}

template <class DataType>
simple_linked_list<DataType>::simple_linked_list(
    const simple_linked_list &other)
    : size_(other.size_) {
  if (size_ == 0) {
    head_ = nullptr;
  } else {

    head_ = new node{other.head_->data, nullptr};
    node *other_node = other.head_->next;
    node *this_node = head_;

    while (other_node != nullptr) {
      this_node->next = new node{other_node->data, nullptr};
      this_node = this_node->next;
      other_node = other_node->next;
    }
  }
}

template <class DataType> simple_linked_list<DataType>::~simple_linked_list() {
  node *tmp;
  while (head_ != nullptr) {
    tmp = head_->next;
    delete head_;
    head_ = tmp;
  }
}

template <class DataType>
std::size_t simple_linked_list<DataType>::size() const {
  return size_;
}

template <class DataType> void simple_linked_list<DataType>::clear() {
  node *temp;
  while (head_ != nullptr) {
    temp = head_->next;
    delete head_;
    head_ = temp;
  }
  size_ = 0;
}

template <class DataType>
typename simple_linked_list<DataType>::iterator
simple_linked_list<DataType>::begin() const {
  return iterator{head_};
}

template <class DataType>
typename simple_linked_list<DataType>::iterator
simple_linked_list<DataType>::end() const {
  return iterator{nullptr};
}

template <class DataType> void simple_linked_list<DataType>::print() const {
  node *node = head_;
  while (node != nullptr) {
    std::cout << node->data << " -> ";
    node = node->next;
  }
  std::cout << "NULL" << std::endl;
}

} // namespace container
} // namespace brocolio
