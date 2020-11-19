#pragma once
#include <iostream>

namespace brocolio::container {

template <class DataType> class linked_list {
public:
  class iterator;
  linked_list() = default;
  linked_list(linked_list const&);
  linked_list(linked_list&&); // TODO
  ~linked_list();
  void insert(DataType const data, std::size_t const position = 0);
  void remove(std::size_t const position = 0);
  iterator begin() const;
  iterator end() const;
  std::size_t size() const;
  void clear();
  void print() const;

private:
  struct node;
  node* head_{nullptr};
  std::size_t size_{0};
};

template <class DataType> struct linked_list<DataType>::node {
  DataType data{};
  node* next{nullptr};
};

template <class DataType> class linked_list<DataType>::iterator {
public:
  iterator() = default;
  iterator(iterator const&) = default;
  iterator(node* node) : node_(node){};
  ~iterator() = default;
  iterator& operator++() {
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
  DataType& operator*() const { return (this->node_)->data; }
  bool operator==(iterator const& other) const {
    return (this->node_ == other.node_);
  }
  bool operator!=( iterator const& other) const { return not(*this == other); }

private:
  node* node_{nullptr};
};

template <class DataType>
void linked_list<DataType>::insert(DataType const data,
                                   std::size_t const position) {
  if (position < size_) {
    if (position == 0) {
      head_ = new node(data, head_);
    } else {
      node* tmp{head_};

      for (std::size_t i{0}; i < position - 1; ++i) {
        tmp = tmp->next;
      }
      tmp->next = new node(data, tmp->next);
    }
  } else {
    if (size_ == 0) {
      head_ = new node(data, nullptr);
    } else {
      node* tmp{head_};
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
void linked_list<DataType>::remove(std::size_t const position) {
  if (position < size_) {
    node* previous_node{head_};
    node* current_node{previous_node->next};

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
    --size_;
  }
}

template <class DataType>
linked_list<DataType>::linked_list(linked_list const& other)
    : size_(other.size_) {
  if (size_ != 0) {
    head_ = new node{other.head_->data, nullptr};
    node* other_node{other.head_->next};
    node* this_node{head_};

    while (other_node != nullptr) {
      this_node->next = new node{other_node->data, nullptr};
      this_node = this_node->next;
      other_node = other_node->next;
    }
  }
}

template <class DataType> linked_list<DataType>::~linked_list() { clear(); }

template <class DataType> std::size_t linked_list<DataType>::size() const {
  return size_;
}

template <class DataType> void linked_list<DataType>::clear() {
  node* tmp{nullptr};
  while (head_ != nullptr) {
    tmp = head_->next;
    delete head_;
    head_ = tmp;
  }
  size_ = 0;
}

template <class DataType>
typename linked_list<DataType>::iterator linked_list<DataType>::begin() const {
  return iterator{head_};
}

template <class DataType>
typename linked_list<DataType>::iterator linked_list<DataType>::end() const {
  return iterator{nullptr};
}

template <class DataType> void linked_list<DataType>::print() const {
  node* node{head_};
  while (node != nullptr) {
    std::cout << node->data << " -> ";
    node = node->next;
  }
  std::cout << "NULL" << std::endl;
}

} // namespace brocolio::container
