#pragma once
#include <iostream>
#include <iterator>
#include <stdexcept>
namespace brocolio::container {

template <class DataType> class stack {
public:
  stack() = default;
  stack(stack const&); // TODO
  stack(stack&&);      // TODO
  ~stack();
  void push(DataType const data);
  DataType pop();
  DataType top() const;
  std::size_t size() const;
  bool empty() const;

private:
  struct node;
  node const* top_{nullptr};
  std::size_t size_{0};
};

template <class DataType> struct stack<DataType>::node {
  DataType const data{};
  node const* next{nullptr};
};

template <class DataType> stack<DataType>::~stack() {
  for (node const* tmp{top_}; top_ != nullptr;) {
    tmp = top_->next;
    delete top_;
    top_ = tmp;
  }
}

template <class DataType> void stack<DataType>::push(DataType const data) {
  top_ = (size_ == 0) ? new node{data, nullptr} : new node{data, top_};
  ++size_;
}

template <class DataType> DataType stack<DataType>::pop() {
  if (size_ != 0) {
    node const* next{top_->next};
    DataType const data{top_->data};
    delete top_;
    top_ = next;
    --size_;
    return data;
  } else {
    throw std::out_of_range{"empty stack"};
  }
}

template <class DataType> DataType stack<DataType>::top() const {
  if (size_ != 0) {
    return top_->data;
  } else {
    throw std::out_of_range{"empty stack"};
  }
}

template <class DataType> std::size_t stack<DataType>::size() const {
  return size_;
}

template <class DataType> bool stack<DataType>::empty() const {
  return size_ == 0;
}

} // namespace brocolio::container
