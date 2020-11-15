#pragma once

#include <functional>
#include <iostream>

namespace brocolio::container {
// TODO add move and copy assignment operators
template <class DataType, class PriorityType> class priority_queue {
public:
  priority_queue() = default;
  priority_queue(const priority_queue &);
  priority_queue(priority_queue &&);
  priority_queue(
      std::function<bool(const PriorityType &, const PriorityType &)> compare);
  ~priority_queue();

  void enqueue(const DataType data, const PriorityType priority);
  DataType dequeue();
  const DataType &front() const;
  void print() const;
  const std::size_t &size() const;

private:
  struct node;
  node *front_{nullptr};
  node *back_{nullptr};
  std::size_t size_{0};
  const std::function<bool(const PriorityType &, const PriorityType &)>
      compare_{[](const PriorityType &, const PriorityType &) { return true; }};
};

template <class DataType, class PriorityType>
struct priority_queue<DataType, PriorityType>::node {
  DataType data{};
  PriorityType priority{};
  node *next{nullptr};
  node() = default;
  node(node &) = default;
  node(const DataType data, const PriorityType priority, node *next)
      : data(data), priority(priority), next(next) {}
  ~node() = default;
};

template <class DataType, class PriorityType>
priority_queue<DataType, PriorityType>::priority_queue(
    const priority_queue &other)
    : compare_(other.compare_), size_(other.size_) {
  if (other.front_ != nullptr) {
    front_ = new node{other.front_->data, other.front_->priority, nullptr};
    back_ = {front_};
    node *other_node{other.front_->next};
    while (other_node != nullptr) {
      back_->next = new node(other_node->data, other_node->priority, nullptr);
      back_ = back_->next;
      other_node = other_node->next;
    }
  }
}

template <class DataType, class PriorityType>
priority_queue<DataType, PriorityType>::priority_queue(priority_queue &&other)
    : front_(other.front_), back_(other.back_), size_(other.size_),
      compare_(other.compare_) {
  other.front_ = nullptr;
  other.back_ = nullptr;
  other.size_ = 0;
}

template <class DataType, class PriorityType>
priority_queue<DataType, PriorityType>::priority_queue(
    const std::function<bool(const PriorityType &, const PriorityType &)>
        compare)
    : compare_(compare) {}

template <class DataType, class PriorityType>
priority_queue<DataType, PriorityType>::~priority_queue() {
  node *tmp = front_;
  while (front_ != nullptr) {
    tmp = front_->next;
    delete front_;
    front_ = tmp;
  }
}

template <class DataType, class PriorityType>
void priority_queue<DataType, PriorityType>::enqueue(
    const DataType data, const PriorityType priority) {
  if (size_ == 0) {
    front_ = new node{data, priority, nullptr};
    back_ = front_;
  } else {
    node *previous_node{nullptr};
    node *current_node{front_};
    bool node_inserted{false};
    while (current_node != nullptr and not node_inserted) {
      if (compare_(priority, current_node->priority)) {
        if (current_node == front_) {
          front_ = new node{data, priority, front_};
        } else {
          previous_node->next = new node{data, priority, current_node};
        }
        node_inserted = true;
      } else {
        previous_node = current_node;
        current_node = current_node->next;
      }
    }
    if (not node_inserted) {
      back_->next = new node{data, priority, nullptr};
      back_ = back_->next;
    }
  }
  ++size_;
}

template <class DataType, class PriorityType>
DataType priority_queue<DataType, PriorityType>::dequeue() {
  if (size_ != 0) {
    node *next = front_->next;
    DataType data = front_->data;
    delete front_;
    front_ = next;
    --size_;
    return data;
  } else {
    std::cout << "warning,empty queue" << std::endl;
    return DataType{};
  }
}

template <class DataType, class PriorityType>
const DataType &priority_queue<DataType, PriorityType>::front() const {
  return front_->data;
}

template <class DataType, class PriorityType>
void priority_queue<DataType, PriorityType>::print() const {
  node *node = front_;
  while (node != nullptr) {
    std::cout << node->data << std::endl;
    node = node->next;
  }
}

template <class DataType, class PriorityType>
const std::size_t &priority_queue<DataType, PriorityType>::size() const {
  return size_;
}
} // namespace brocolio::container
