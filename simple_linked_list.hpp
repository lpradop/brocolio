#pragma once

#include "simple_list_iterator.hpp"
#include "simple_node.hpp"
#include <iostream>
#include <utility>

// TODO mejorar la funcion search, implementar std::pair
template <class DataType> class SimpleLinkedList {
private:
  SimpleNode<DataType> *head_;
  std::size_t size_;

public:
  SimpleLinkedList();
  ~SimpleLinkedList();
  SimpleLinkedList(const SimpleLinkedList<DataType> &other);
  void Insert(const DataType data, const std::size_t position = 0);
  void Remove(const std::size_t position = 0);

  // TODO refactor
  template <class... Args>
  SimpleLinkedList<std::pair<DataType, unsigned int>>
  Search(bool (*Criteria)(const DataType, Args...), Args... args);

  SimpleLinkedListIterator<DataType> Begin() const;
  SimpleLinkedListIterator<DataType> End() const;
  std::size_t GetSize() const;
  void Clear();
  void Print() const;

  DataType &operator[](const std::size_t index) const {
    if (index >= 0 and index < size_) {
      auto node = head_;
      std::size_t counter = 0;
      while (node != nullptr and counter < index) {
        ++counter;
        node = node->next_;
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
      head_ = new SimpleNode<DataType>(data, head_);
    } else {
      SimpleNode<DataType> *node = head_;

      for (unsigned int i = 0; i < position - 1; ++i) {
        node = node->next_;
      }
      node->next_ = new SimpleNode<DataType>(data, node->next_);
    }
  } else {
    if (size_ == 0) {
      head_ = new SimpleNode<DataType>(data, nullptr);
    } else {
      SimpleNode<DataType> *node = head_;
      while (node != nullptr) {
        if (node->next_ == nullptr)
          break;
        node = node->next_;
      }
      node->next_ = new SimpleNode<DataType>(data, nullptr);
    }
  }
  ++size_;
}

template <class DataType>
template <class... Args>
SimpleLinkedList<std::pair<DataType, unsigned int>>
SimpleLinkedList<DataType>::Search(bool (*Criteria)(const DataType, Args...),
                                   Args... args) {
  SimpleLinkedList<std::pair<DataType, unsigned int>> found_list{};
  SimpleNode<DataType> *node = head_;
  unsigned int position = 0;

  while (node != nullptr) {
    if (Criteria(node->data_, args...)) {
      DataType data = node->data_;
      found_list.Insert(std::make_pair(data, position), 0);
    }

    node = node->next_;
    ++position;
  }
  return found_list;
}

template <class DataType>
void SimpleLinkedList<DataType>::Remove(std::size_t position) {
  if (position < size_) {
    SimpleNode<DataType> *previous_node = head_;
    SimpleNode<DataType> *current_node = previous_node->next_;

    if (position == 0) {
      head_ = current_node;
      delete previous_node;
    } else {

      for (unsigned int i = 0; i < position - 1; ++i) {
        previous_node = current_node;
        current_node = current_node->next_;
      }

      previous_node->next_ = current_node->next_;
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

    head_ = new SimpleNode<DataType>{other.head_->data_, nullptr};
    SimpleNode<DataType> *other_node = other.head_->next_;
    SimpleNode<DataType> *this_node = head_;

    while (other_node != nullptr) {
      this_node->next_ = new SimpleNode<DataType>{other_node->data_, nullptr};
      this_node = this_node->next_;
      other_node = other_node->next_;
    }
  }
}

template <class DataType>
SimpleLinkedList<DataType>::SimpleLinkedList() : size_(0), head_(nullptr) {}

template <class DataType> SimpleLinkedList<DataType>::~SimpleLinkedList() {
  SimpleNode<DataType> *tmp;
  while (head_ != nullptr) {
    tmp = head_->next_;
    delete head_;
    head_ = tmp;
  }
}
template <class DataType>
std::size_t SimpleLinkedList<DataType>::GetSize() const {
  return size_;
}

template <class DataType> void SimpleLinkedList<DataType>::Clear() {
  SimpleNode<DataType> *temp;
  while (head_ != nullptr) {
    temp = head_->next_;
    delete head_;
    head_ = temp;
  }
  size_ = 0;
}

template <class DataType>
SimpleLinkedListIterator<DataType> SimpleLinkedList<DataType>::Begin() const {
  return SimpleLinkedListIterator<DataType>{head_};
}

template <class DataType>
SimpleLinkedListIterator<DataType> SimpleLinkedList<DataType>::End() const {
  return SimpleLinkedListIterator<DataType>{nullptr};
}

template <class DataType> void SimpleLinkedList<DataType>::Print() const {

  SimpleNode<DataType> *node = head_;
  while (node != nullptr) {
    std::cout << node->data_ << " -> ";
    node = node->next_;
  }
  std::cout << "NULL" << std::endl;
}
