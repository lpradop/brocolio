#include <bits/c++config.h>
#include <iostream>

namespace brocolio::container {
template <class DataType> class vector {
public:
  vector();
  ~vector();
  bool empty() const;
  std::size_t size() const;
  std::size_t capacity() const;
  void reserve(std::size_t const capacity);
  void clear();

private:
  static std::size_t constexpr initial_capacity_{4};
  DataType* c_array_{nullptr};
  std::size_t size_{0};
  std::size_t capacity_{0};
};

template <class DataType>
vector<DataType>::vector()
    : c_array_(new DataType[initial_capacity_]), capacity_(initial_capacity_) {}

template <class DataType> vector<DataType>::~vector() { delete[] c_array_; }
} // namespace brocolio::container
