#pragma once
#include <initializer_list>
#include <stdexcept>
namespace brocolio {
namespace container {

template <class DataType, std::size_t array_size> class array {
public:
  class iterator;
  class const_iterator;
  array() = default;
  array(const array &);
  array(const std::initializer_list<DataType> values);
  ~array() = default;
  std::size_t size() const { return array_size; };
  DataType &operator[](const std::size_t index);

private:
  DataType c_array_[array_size]{};
};

template <class DataType, std::size_t array_size>
array<DataType, array_size>::array(const array &other) {
  if (this->size() == other.size()) {
    for (std::size_t i = 0; i < array_size; ++i) {
      c_array_[i] = other.c_array_[i];
    }
  } else {
    throw std::length_error{"sizes of arrays are not the same"};
  }
}

template <class DataType, std::size_t array_size>
array<DataType, array_size>::array(
    const std::initializer_list<DataType> values) {
  if (values.size() == array_size) {
    std::size_t i = 0;
    for (auto &v : values) {
      c_array_[i] = v;
      ++i;
    };
  } else {
    throw std::length_error{
        "size of initialializer list doesn't match with array declaration"};
  }
}

template <class DataType, std::size_t array_size>
DataType &array<DataType, array_size>::operator[](const std::size_t index) {
  if (index < array_size) {
    return c_array_[index];
  } else {
    throw std::out_of_range{"trying to access an element out of bounds"};
  }
}

} // namespace container
} // namespace brocolio
