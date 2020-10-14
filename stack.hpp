#pragma once

#include <iostream>
namespace brocolio {
namespace container {

template <class DataType> class stack {
private:
  struct node;
  node *top_{nullptr};
  std::size_t size_{0};

public:
  stack() = default;
  ~stack();
  void push(DataType data);
  DataType pop();
  DataType top() const;
  std::size_t size() const;
};

template <class DataType> stack<DataType>::~stack() {
  node *tmp = top_;
  while (top_ != nullptr) {
    tmp = top_->next;
    delete top_;
    top_ = tmp;
  }
}

template <class DataType> void stack<DataType>::push(DataType data) {
  if (size_ == 0) {
    top_ = new node{data, nullptr};
  } else {
    top_ = new node{data, top_};
  }
  ++size_;
}

template <class DataType> DataType stack<DataType>::pop() {
  if (size_ != 0) {
    node *next = top_->next;
    DataType data = top_->data;
    delete top_;
    top_ = next;
    --size_;
    return data;
  } else {
    std::cout << "warning,empty stack" << std::endl;
    return DataType{};
  }
}

template <class DataType> DataType stack<DataType>::top() const {

  if (size_ != 0) {

    return top_->data;
  } else {
    std::cout << "warning,empty stack" << std::endl;
    return DataType{};
  }
}

template <class DataType> std::size_t stack<DataType>::size() const {
  return size_;
}

template <class DataType> struct stack<DataType>::node {
  DataType data{};
  node *next{nullptr};
  node() = default;
  node(DataType data, node *next) : data(data), next(next) {}
  ~node() = default;
};

} // namespace container
} // namespace brocolio
