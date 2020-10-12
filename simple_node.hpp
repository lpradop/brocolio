#pragma once
// SimpleNode template by Brocolio de la CHUNSA
// This template only have a reference to the next node, use DoublyNode if you
// want previous node reference
template <class DataType> struct SimpleNode {
  DataType data;
  SimpleNode<DataType> *next;
  SimpleNode() = delete;
  SimpleNode(DataType data = DataType{}, SimpleNode<DataType> *next = nullptr);
  ~SimpleNode() = default;
};

template <class DataType>
SimpleNode<DataType>::SimpleNode(DataType data, SimpleNode *next)
    : data(data), next(next) {}
