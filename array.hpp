#pragma once
#include <initializer_list>
#include <stdexcept>
namespace brocolio {
namespace container {

template <class DataType, std::size_t array_size> class array {
public:
  static_assert(array_size > 0, "array must be at least size 1");
  class iterator;
  class const_iterator;
  array() : c_array_(new DataType[array_size]) {}
  array(const array &);
  array(array &&);
  array(const DataType (&array)[array_size]);
  array(const std::initializer_list<DataType> il);
  ~array();
  constexpr std::size_t size() const { return array_size; }
  DataType &operator[](const std::size_t index);
  const DataType &operator[](const std::size_t index) const;
  array &operator=(const array &);
  array &operator=(array &&);
  iterator begin();
  iterator end();
  const_iterator cbegin() const;
  const_iterator cend() const;

private:
  DataType *c_array_{nullptr};
};

template <class DataType, std::size_t array_size>
class array<DataType, array_size>::iterator {
public:
  iterator() = default;
  iterator(const iterator &) = default;
  iterator(const array &container, const std::size_t index,
           const bool end_flag = false);
  ~iterator() = default;
  DataType &operator*();
  bool operator==(const iterator &) const;
  bool operator!=(const iterator &) const;
  iterator &operator++();
  iterator &operator--();
  iterator &operator++(int);

private:
  const array &container_;
  std::size_t index_{0};
  bool end_flag_{false};
};

template <class DataType, std::size_t array_size>
class array<DataType, array_size>::const_iterator {
public:
  const_iterator() = default;
  const_iterator(const const_iterator &) = default;
  const_iterator(const array &container, const std::size_t index,
                 const bool end_flag = false);
  ~const_iterator() = default;
  const DataType &operator*() const;
  bool operator==(const const_iterator &) const;
  bool operator!=(const const_iterator &) const;
  const_iterator &operator++();
  const_iterator &operator--();
  const_iterator &operator++(int);

private:
  const array &container_;
  std::size_t index_{0};
  bool end_flag_{false};
};

template <class DataType, std::size_t array_size>
array<DataType, array_size>::iterator::iterator(const array &container,
                                                const std::size_t index,
                                                const bool end_flag)
    : container_(container), index_(index), end_flag_(end_flag) {}

template <class DataType, std::size_t array_size>
DataType &array<DataType, array_size>::iterator::operator*() {
  return container_.c_array_[index_];
}

template <class DataType, std::size_t array_size>
bool array<DataType, array_size>::iterator::operator==(
    const iterator &other) const {
  if (end_flag_ and other.end_flag_) {
    return true;
  } else if (not(end_flag_ or other.end_flag_)) {
    return (container_.c_array_ + index_) ==
           (other.container_.c_array_ + index_);
  } else {
    return false;
  }
}

template <class DataType, std::size_t array_size>
bool array<DataType, array_size>::iterator::operator!=(
    const iterator &other) const {
  return not(*this == other);
}

template <class DataType, std::size_t array_size>
typename array<DataType, array_size>::iterator &
array<DataType, array_size>::iterator::operator++() {
  if ((index_ < array_size) and (not end_flag_)) {
    ++index_;
    if (index_ == array_size) {
      {
        end_flag_ = true;
      }
    }
  }
  return *this;
}

template <class DataType, std::size_t array_size>
array<DataType, array_size>::const_iterator::const_iterator(
    const array &container, const std::size_t index, const bool end_flag)
    : container_(container), index_(index), end_flag_(end_flag) {}

template <class DataType, std::size_t array_size>
const DataType &array<DataType, array_size>::const_iterator::operator*() const {
  return container_.c_array_[index_];
}

template <class DataType, std::size_t array_size>
bool array<DataType, array_size>::const_iterator::operator==(
    const const_iterator &other) const {
  if (end_flag_ and other.end_flag_) {
    return true;
  } else if (not(end_flag_ or other.end_flag_)) {
    return (container_.c_array_ + index_) ==
           (other.container_.c_array_ + index_);
  } else {
    return false;
  }
}

template <class DataType, std::size_t array_size>
bool array<DataType, array_size>::const_iterator::operator!=(
    const const_iterator &other) const {
  return not(*this == other);
}

template <class DataType, std::size_t array_size>
typename array<DataType, array_size>::const_iterator &
array<DataType, array_size>::const_iterator::operator++() {
  if ((index_ < array_size) and (not end_flag_)) {
    ++index_;
    if (index_ == array_size) {
      {
        end_flag_ = true;
      }
    }
  }
  return *this;
}

template <class DataType, std::size_t array_size>
array<DataType, array_size>::array(const array &other)
    : c_array_(new DataType[array_size]) {
  if (this->size() == other.size()) {
    for (std::size_t i = 0; i < array_size; ++i) {
      c_array_[i] = other.c_array_[i];
    }
  } else {
    throw std::length_error{"sizes of arrays are not the same"};
  }
}

template <class DataType, std::size_t array_size>
array<DataType, array_size>::array(array &&other) : c_array_(other.c_array_) {
  other.c_array_ = nullptr;
}

template <class DataType, std::size_t array_size>
array<DataType, array_size> &
array<DataType, array_size>::operator=(const array &other) {
  if (this->size() == other.size()) {
    for (std::size_t i = 0; i < array_size; ++i) {
      c_array_[i] = other.c_array_[i];
    }
    return *this;
  } else {
    throw std::length_error{"sizes of arrays are not the same"};
  }
}

template <class DataType, std::size_t array_size>
array<DataType, array_size> &
array<DataType, array_size>::operator=(array &&other) {
  c_array_ = other.c_array_;
  other.c_array_ = nullptr;
}

template <class DataType, std::size_t array_size>
array<DataType, array_size>::array(const DataType (&array)[array_size])
    : c_array_(new DataType[array_size]) {
  for (std::size_t i = 0; i < array_size; ++i) {
    c_array_[i] = array[i];
  }
}

template <class DataType, std::size_t array_size>
array<DataType, array_size>::array(const std::initializer_list<DataType> il)
    : c_array_(new DataType[array_size]) {
  if (size() == il.size()) {
    std::size_t i = 0;
    for (auto &e : il) {
      c_array_[i] = e;
      ++i;
    }
  } else {
    throw std::length_error{"sizes of arrays are not the same"};
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

template <class DataType, std::size_t array_size>
const DataType &
array<DataType, array_size>::operator[](const std::size_t index) const {
  if (index < array_size) {
    return c_array_[index];
  } else {
    throw std::out_of_range{"trying to access an element out of bounds"};
  }
}

template <class DataType, std::size_t array_size>
array<DataType, array_size>::~array() {
  delete[] c_array_;
  c_array_ = nullptr;
}

template <class DataType, std::size_t array_size>
typename array<DataType, array_size>::iterator
array<DataType, array_size>::begin() {
  return iterator{*this, 0};
}

template <class DataType, std::size_t array_size>
typename array<DataType, array_size>::iterator
array<DataType, array_size>::end() {
  return iterator{*this, array_size, true};
}

template <class DataType, std::size_t array_size>
typename array<DataType, array_size>::const_iterator
array<DataType, array_size>::cbegin() const {
  return const_iterator{*this, 0};
}

template <class DataType, std::size_t array_size>
typename array<DataType, array_size>::const_iterator
array<DataType, array_size>::cend() const {
  return const_iterator{*this, array_size, true};
}

} // namespace container
} // namespace brocolio
