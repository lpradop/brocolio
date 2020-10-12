#pragma once
#include "simple_node.hpp"
namespace brocolio {
namespace container {
template <class DataType> class Iterator {
private:
  template <class T> friend class SimpleLinkedList;
  SimpleNode<DataType> *node_;

  Iterator(SimpleNode<DataType> *node = nullptr)
      : node_(node){};

public:
  ~Iterator() = default;
  Iterator<DataType> &operator++() {
    if (node_ != nullptr) {
      node_ = node_->next_;
    }
    return *this;
  };

  Iterator<DataType> &operator++(int) {
    if (node_ != nullptr) {
      Iterator<DataType> tmp(*this);
      operator++();
      return tmp;
    } else {
      return *this;
    }
  }

  DataType &operator*() const { return (this->node_)->data_; }

  bool operator==(const Iterator<DataType> &obj) const {
    return (this->node_ == obj.node_);
  }

  bool operator!=(const Iterator<DataType> &obj) const {
    return not(*this == obj);
  }
};
} // namespace container
} // namespace brocolio
