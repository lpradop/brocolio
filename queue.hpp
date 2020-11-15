#pragma once
#include <iostream>
namespace brocolio::container {

template <class DataType> class queue {
public:
  queue() = default;
  queue(const queue &);
  queue(queue &&);
  ~queue();
  void enqueue(const DataType data);
  DataType dequeue();
  DataType front() const;
  void print() const;
  std::size_t size() const;

private:
  struct node;
  node *front_{nullptr};
  node *back_{nullptr};
  std::size_t size_{0};
};

template <class DataType> queue<DataType>::~queue() {
  node *tmp{front_};
  while (front_ != nullptr) {
    tmp = front_->next;
    delete front_;
    front_ = tmp;
  }
}

template <class DataType> void queue<DataType>::enqueue(DataType data) {
  if (front_ == nullptr) {
    front_ = new node{data, nullptr};
    back_ = front_;

  } else {
    back_->next = new node{data, nullptr};
    back_ = back_->next;
  }
  ++size_;
}

template <class DataType> DataType queue<DataType>::dequeue() {
  if (size_ != 0) {
    node *next{front_->next};
    DataType data{front_->data};
    delete front_;
    front_ = next;
    --size_;
    return data;
  } else {
    std::cout << "warning,empty queue" << std::endl;
    return DataType{};
  }
}

template <class DataType> DataType queue<DataType>::front() const {
  return front_->data;
}

template <class DataType> void queue<DataType>::print() const {
  node *node{front_};
  while (node != nullptr) {
    std::cout << node->data << std::endl;
    node = node->next;
  }
}

template <class DataType> std::size_t queue<DataType>::size() const {
  return size_;
}

template <class DataType> struct queue<DataType>::node {
  DataType data{};
  node *next{nullptr};
  node() = default;
  node(DataType data, node *next) : data(data), next(next) {}
  ~node() = default;
};
} // namespace brocolio::container
