#pragma once
#include <initializer_list>
#include <stdexcept>
namespace brocolio::container {
// array class wrapper by Brocolio de la CHUNSA
// TODO add type casting
template <class DataType, std::size_t Size>
class array {
public:
  static_assert(Size > 0, "array size must be at least 1");
  class iterator;
  array();
  array(array const&);
  array(array&&);
  array(std::initializer_list<DataType> const il);
  array(DataType const (&arr)[Size]);
  ~array();
  std::size_t constexpr size() const { return Size; }
  DataType& operator[](std::size_t const index);
  DataType const& operator[](std::size_t const index) const;
  array& operator=(array const&);
  array& operator=(array&&);
  iterator begin();
  iterator end();

private:
  DataType* c_array_{nullptr};
};

template <class DataType, std::size_t array_size>
class array<DataType, array_size>::iterator {
public:
  iterator() = default;
  iterator(iterator const&) = default;
  iterator(array* container, std::size_t const index,
           bool const end_flag = false);
  ~iterator() = default;
  iterator& operator=(iterator const&) const = default;
  iterator& operator=(iterator&) = default;
  DataType& operator*();
  bool operator==(iterator const&) const;
  bool operator!=(iterator const&) const;
  iterator& operator++();
  iterator& operator--();
  iterator& operator++(int);

private:
  array* container_;
  std::size_t index_{0};
  bool end_flag_{false};
};

template <class DataType, std::size_t Size>
array<DataType, Size>::iterator::iterator(array* container,
                                          std::size_t const index,
                                          bool const end_flag)
    : container_(container), index_(index), end_flag_(end_flag) {}

template <class DataType, std::size_t Size>
DataType& array<DataType, Size>::iterator::operator*() {
  return container_->c_array_[index_];
}

template <class DataType, std::size_t Size>
bool array<DataType, Size>::iterator::operator==(iterator const& other) const {
  if (end_flag_ and other.end_flag_) {
    return true;
  } else if (not(end_flag_ or other.end_flag_)) {
    return ((container_->c_array_ + index_) ==
            (other.container_->c_array_ + index_));
  } else {
    return false;
  }
}

template <class DataType, std::size_t Size>
bool array<DataType, Size>::iterator::operator!=(iterator const& other) const {
  return not(*this == other);
}

template <class DataType, std::size_t Size>
typename array<DataType, Size>::iterator&
array<DataType, Size>::iterator::operator++() {
  if ((index_ < Size) and (not end_flag_)) {
    ++index_;
    if (index_ == Size) {
      end_flag_ = true;
    }
  }
  return *this;
}

template <class DataType, std::size_t Size>
array<DataType, Size>::array() : c_array_(new DataType[Size]{}) {}

template <class DataType, std::size_t Size>
array<DataType, Size>::array(array const& other)
    : c_array_(new DataType[Size]{}) {
  if (this->size() == other.size()) {
    for (std::size_t i = 0; i < Size; ++i) {
      c_array_[i] = other.c_array_[i];
    }
  } else {
    throw std::length_error{"sizes of arrays are not the same"};
  }
}

template <class DataType, std::size_t Size>
array<DataType, Size>::array(array&& other) : c_array_(other.c_array_) {
  other.c_array_ = nullptr;
}

template <class DataType, std::size_t Size>
array<DataType, Size>& array<DataType, Size>::operator=(array const& other) {
  if (this->size() == other.size()) {
    for (std::size_t i = 0; i < Size; ++i) {
      c_array_[i] = other.c_array_[i];
    }
    return *this;
  } else {
    throw std::length_error{"sizes of arrays are not the same"};
  }
}

template <class DataType, std::size_t Size>
array<DataType, Size>& array<DataType, Size>::operator=(array&& other) {
  c_array_ = other.c_array_;
  other.c_array_ = nullptr;
}

template <class DataType, std::size_t Size>
array<DataType, Size>::array(std::initializer_list<DataType> const il)
    : array() {
  if (size() == il.size()) {
    std::size_t i = 0;
    for (auto& e : il) {
      c_array_[i] = e;
      ++i;
    }
  } else {
    throw std::length_error{"sizes of arrays are not the same"};
  }
}

template <class DataType, std::size_t Size>
array<DataType, Size>::array(DataType const (&arr)[Size]) : array() {
  for (std::size_t i = 0; i < Size; ++i) {
    c_array_[i] = arr[i];
  }
}

template <class DataType, std::size_t Size>
DataType& array<DataType, Size>::operator[](std::size_t const index) {
  if (index < Size) {
    return c_array_[index];
  } else {
    throw std::out_of_range{"trying to access an element out of bounds"};
  }
}

template <class DataType, std::size_t Size>
DataType const&
array<DataType, Size>::operator[](std::size_t const index) const {
  if (index < Size) {
    return c_array_[index];
  } else {
    throw std::out_of_range{"trying to access an element out of bounds"};
  }
}

template <class DataType, std::size_t Size>
array<DataType, Size>::~array() {
  delete[] c_array_;
}

template <class DataType, std::size_t Size>
typename array<DataType, Size>::iterator array<DataType, Size>::begin() {
  return iterator{this, 0};
}

template <class DataType, std::size_t Size>
typename array<DataType, Size>::iterator array<DataType, Size>::end() {
  return iterator{this, Size, true};
}

} // namespace brocolio::container
