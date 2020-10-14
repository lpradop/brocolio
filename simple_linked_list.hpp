#pragma once

#include "ordered_pair.hpp"
#include <iostream>
#include <utility>
namespace brocolio {
namespace container {

// TODO mejorar la funcion search
template <class DataType> class simple_linked_list {
private:
  struct node;
  node *head_;
  std::size_t size_;

public:
  class iterator;
  simple_linked_list();
  ~simple_linked_list();
  simple_linked_list(const simple_linked_list<DataType> &other);
  void insert(const DataType data, const std::size_t position = 0);
  void remove(const std::size_t position = 0);

  // TODO refactor
  // template <class... Args>
  // SimpleLinkedList<std::pair<DataType, unsigned int>>
  // Search(bool (*Criteria)(const DataType, Args...), Args... args);

  iterator begin() const;
  iterator end() const;
  std::size_t size() const;
  void clear();
  void print() const;

  DataType &operator[](const std::size_t index) const {
    if (index >= 0 and index < size_) {
      auto tmp = head_;
      std::size_t counter = 0;
      while (tmp != nullptr and counter < index) {
        ++counter;
        tmp = tmp->next;
      }
      return tmp->data_;
    } else {
      std::cout << "Element out of bounds" << std::endl;
      return DataType{};
    }
  }
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

// template <class DataType>
// template <class... Args>
// SimpleLinkedList<std::pair<DataType, unsigned int>>
// SimpleLinkedList<DataType>::Search(bool (*Criteria)(const DataType, Args...),
//                                    Args... args) {
//   SimpleLinkedList<std::pair<DataType, unsigned int>> found_list{};
//   Node *node = head_;
//   unsigned int position = 0;

//   while (node != nullptr) {
//     if (Criteria(node->data_, args...)) {
//       DataType data = node->data_;
//       found_list.Insert(std::make_pair(data, position), 0);
//     }

//     node = node->next;
//     ++position;
//   }
//   return found_list;
// }

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

    head_ = new node{other.head_->data_, nullptr};
    node *other_node = other.head_->next;
    node *this_node = head_;

    while (other_node != nullptr) {
      this_node->next = new node{other_node->data_, nullptr};
      this_node = this_node->next;
      other_node = other_node->next;
    }
  }
}

template <class DataType>
simple_linked_list<DataType>::simple_linked_list() : size_(0), head_(nullptr) {}

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

template <class DataType> class simple_linked_list<DataType>::iterator {
private:
  node *node_;
  iterator(node *node = nullptr) : node_(node){};

  template <class T> friend class SimpleLinkedList;

public:
  ~iterator() = default;
  iterator &operator++() {
    if (node_ != nullptr) {
      node_ = node_->next;
    }
    return *this;
  };
  iterator &operator++(int) {
    if (node_ != nullptr) {
      iterator tmp(*this);
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

template <class DataType> struct simple_linked_list<DataType>::node {
  DataType data;
  node *next;
  node() = delete;
  node(DataType data = DataType{}, node *next = nullptr)
      : data(data), next(next) {}
  ~node() = default;
};
} // namespace container
} // namespace brocolio
