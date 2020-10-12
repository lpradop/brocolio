#pragma once
#include "simple_node.hpp"
template <class DataType> class SimpleLinkedListIterator {
private:
  template <class T> friend class SimpleLinkedList;
  SimpleNode<DataType> *node_;

  SimpleLinkedListIterator(SimpleNode<DataType> *node = nullptr)
      : node_(node){};

public:
  ~SimpleLinkedListIterator() = default;
  SimpleLinkedListIterator<DataType> &operator++() {
    if (node_ != nullptr) {
      node_ = node_->next_;
    }
    return *this;
  };

  SimpleLinkedListIterator<DataType> &operator++(int) {
    if (node_ != nullptr) {
      SimpleLinkedListIterator<DataType> tmp(*this);
      operator++();
      return tmp;
    } else {
      return *this;
    }
  }

  DataType &operator*() const { return (this->node_)->data_; }

  bool operator==(const SimpleLinkedListIterator<DataType> &obj) const {
    return (this->node_ == obj.node_);
  }

  bool operator!=(const SimpleLinkedListIterator<DataType> &obj) const {
    return not(*this == obj);
  }
};
