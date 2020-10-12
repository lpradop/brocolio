#pragma once

#include "ordered_pair.hpp"
#include <iostream>
#include <utility>
namespace brocolio {
namespace container {

// TODO mejorar la funcion search
template <class DataType> class SimpleLinkedList {
private:
  struct Node;
  Node *head_;
  std::size_t size_;

public:
  class Iterator;
  SimpleLinkedList();
  ~SimpleLinkedList();
  SimpleLinkedList(const SimpleLinkedList<DataType> &other);
  void Insert(const DataType data, const std::size_t position = 0);
  void Remove(const std::size_t position = 0);

  // TODO refactor
  // template <class... Args>
  // SimpleLinkedList<std::pair<DataType, unsigned int>>
  // Search(bool (*Criteria)(const DataType, Args...), Args... args);

  Iterator Begin() const;
  Iterator End() const;
  std::size_t GetSize() const;
  void Clear();
  void Print() const;

  DataType &operator[](const std::size_t index) const {
    if (index >= 0 and index < size_) {
      auto node = head_;
      std::size_t counter = 0;
      while (node != nullptr and counter < index) {
        ++counter;
        node = node->next;
      }
      return node->data_;
    } else {
      std::cout << "Element out of bounds" << std::endl;
      return DataType{};
    }
  }
};

template <class DataType>
void SimpleLinkedList<DataType>::Insert(const DataType data,
                                        const std::size_t position) {
  if (position < size_) {
    if (position == 0) {
      head_ = new Node(data, head_);
    } else {
      Node *node = head_;

      for (unsigned int i = 0; i < position - 1; ++i) {
        node = node->next;
      }
      node->next = new Node(data, node->next);
    }
  } else {
    if (size_ == 0) {
      head_ = new Node(data, nullptr);
    } else {
      Node *node = head_;
      while (node != nullptr) {
        if (node->next == nullptr)
          break;
        node = node->next;
      }
      node->next = new Node(data, nullptr);
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
void SimpleLinkedList<DataType>::Remove(std::size_t position) {
  if (position < size_) {
    Node *previous_node = head_;
    Node *current_node = previous_node->next;

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
SimpleLinkedList<DataType>::SimpleLinkedList(const SimpleLinkedList &other)
    : size_(other.size_) {
  if (size_ == 0) {
    head_ = nullptr;
  } else {

    head_ = new Node{other.head_->data_, nullptr};
    Node *other_node = other.head_->next;
    Node *this_node = head_;

    while (other_node != nullptr) {
      this_node->next = new Node{other_node->data_, nullptr};
      this_node = this_node->next;
      other_node = other_node->next;
    }
  }
}

template <class DataType>
SimpleLinkedList<DataType>::SimpleLinkedList() : size_(0), head_(nullptr) {}

template <class DataType> SimpleLinkedList<DataType>::~SimpleLinkedList() {
  Node *tmp;
  while (head_ != nullptr) {
    tmp = head_->next;
    delete head_;
    head_ = tmp;
  }
}
template <class DataType>
std::size_t SimpleLinkedList<DataType>::GetSize() const {
  return size_;
}

template <class DataType> void SimpleLinkedList<DataType>::Clear() {
  Node *temp;
  while (head_ != nullptr) {
    temp = head_->next;
    delete head_;
    head_ = temp;
  }
  size_ = 0;
}

template <class DataType>
typename SimpleLinkedList<DataType>::Iterator
SimpleLinkedList<DataType>::Begin() const {
  return Iterator{head_};
}

template <class DataType>
typename SimpleLinkedList<DataType>::Iterator
SimpleLinkedList<DataType>::End() const {
  return Iterator{nullptr};
}

template <class DataType> void SimpleLinkedList<DataType>::Print() const {

  Node *node = head_;
  while (node != nullptr) {
    std::cout << node->data << " -> ";
    node = node->next;
  }
  std::cout << "NULL" << std::endl;
}

template <class DataType> class SimpleLinkedList<DataType>::Iterator {
private:
  Node *node_;
  Iterator(Node *node = nullptr) : node_(node){};

  template <class T> friend class SimpleLinkedList;

public:
  ~Iterator() = default;
  Iterator &operator++() {
    if (node_ != nullptr) {
      node_ = node_->next;
    }
    return *this;
  };
  Iterator &operator++(int) {
    if (node_ != nullptr) {
      Iterator tmp(*this);
      operator++();
      return tmp;
    } else {
      return *this;
    }
  }
  DataType &operator*() const { return (this->node_)->data; }
  bool operator==(const Iterator &obj) const {
    return (this->node_ == obj.node_);
  }
  bool operator!=(const Iterator &obj) const { return not(*this == obj); }
};

template <class DataType> struct SimpleLinkedList<DataType>::Node {
  DataType data;
  Node *next;
  Node() = delete;
  Node(DataType data = DataType{}, Node *next = nullptr)
      : data(data), next(next) {}
  ~Node() = default;
};
} // namespace container
} // namespace brocolio
