#pragma once

#include <initializer_list>
#include <stdexcept>
namespace brocolio::container {
// TODO minor refactoring
template <class DataType, std::size_t Size> class array {
  public:
    static_assert(Size > 0, "array must be at least size 1");
    class iterator;
    class const_iterator;
    array() : c_array_(new DataType[Size]) {}
    array(const array &);
    array(array &&);
    array(const DataType (&array)[Size]);
    array(const std::initializer_list<DataType> il);
    ~array();
    constexpr std::size_t size() const { return Size; }
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

template <class DataType, std::size_t Size>
class array<DataType, Size>::const_iterator {
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

template <class DataType, std::size_t Size>
array<DataType, Size>::iterator::iterator(const array &container,
                                          const std::size_t index,
                                          const bool end_flag)
    : container_(container), index_(index), end_flag_(end_flag) {}

template <class DataType, std::size_t Size>
DataType &array<DataType, Size>::iterator::operator*() {
    return container_.c_array_[index_];
}

template <class DataType, std::size_t Size>
bool array<DataType, Size>::iterator::operator==(const iterator &other) const {
    if (end_flag_ and other.end_flag_) {
        return true;
    } else if (not(end_flag_ or other.end_flag_)) {
        return ((container_.c_array_ + index_) ==
                (other.container_.c_array_ + index_));
    } else {
        return false;
    }
}

template <class DataType, std::size_t Size>
bool array<DataType, Size>::iterator::operator!=(const iterator &other) const {
    return not(*this == other);
}

template <class DataType, std::size_t Size>
typename array<DataType, Size>::iterator &
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
array<DataType, Size>::const_iterator::const_iterator(const array &container,
                                                      const std::size_t index,
                                                      const bool end_flag)
    : container_(container), index_(index), end_flag_(end_flag) {}

template <class DataType, std::size_t Size>
const DataType &array<DataType, Size>::const_iterator::operator*() const {
    return container_.c_array_[index_];
}

template <class DataType, std::size_t Size>
bool array<DataType, Size>::const_iterator::operator==(
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

template <class DataType, std::size_t Size>
bool array<DataType, Size>::const_iterator::operator!=(
    const const_iterator &other) const {
    return not(*this == other);
}

template <class DataType, std::size_t Size>
typename array<DataType, Size>::const_iterator &
array<DataType, Size>::const_iterator::operator++() {
    if ((index_ < Size) and (not end_flag_)) {
        ++index_;
        if (index_ == Size) {
            { end_flag_ = true; }
        }
    }
    return *this;
}

template <class DataType, std::size_t Size>
array<DataType, Size>::array(const array &other)
    : c_array_(new DataType[Size]) {
    if (this->size() == other.size()) {
        for (std::size_t i = 0; i < Size; ++i) {
            c_array_[i] = other.c_array_[i];
        }
    } else {
        throw std::length_error{"sizes of arrays are not the same"};
    }
}

template <class DataType, std::size_t Size>
array<DataType, Size>::array(array &&other) : c_array_(other.c_array_) {
    other.c_array_ = nullptr;
}

template <class DataType, std::size_t Size>
array<DataType, Size> &array<DataType, Size>::operator=(const array &other) {
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
array<DataType, Size> &array<DataType, Size>::operator=(array &&other) {
    c_array_ = other.c_array_;
    other.c_array_ = nullptr;
}

// template <class DataType, std::size_t Size>
// array<DataType, Size>::array(const DataType (&array)[Size])
//     : c_array_(new DataType[Size]) {
//   for (std::size_t i = 0; i < Size; ++i) {
//     c_array_[i] = array[i];
//   }
// }

template <class DataType, std::size_t Size>
array<DataType, Size>::array(const std::initializer_list<DataType> il)
    : c_array_(new DataType[Size]) {
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

template <class DataType, std::size_t Size>
DataType &array<DataType, Size>::operator[](const std::size_t index) {
    if (index < Size) {
        return c_array_[index];
    } else {
        throw std::out_of_range{"trying to access an element out of bounds"};
    }
}

template <class DataType, std::size_t Size>
const DataType &
array<DataType, Size>::operator[](const std::size_t index) const {
    if (index < Size) {
        return c_array_[index];
    } else {
        throw std::out_of_range{"trying to access an element out of bounds"};
    }
}

template <class DataType, std::size_t Size> array<DataType, Size>::~array() {
    delete[] c_array_;
    c_array_ = nullptr;
}

template <class DataType, std::size_t Size>
typename array<DataType, Size>::iterator array<DataType, Size>::begin() {
    return iterator{*this, 0};
}

template <class DataType, std::size_t Size>
typename array<DataType, Size>::iterator array<DataType, Size>::end() {
    return iterator{*this, Size, true};
}

template <class DataType, std::size_t Size>
typename array<DataType, Size>::const_iterator
array<DataType, Size>::cbegin() const {
    return const_iterator{*this, 0};
}

template <class DataType, std::size_t Size>
typename array<DataType, Size>::const_iterator
array<DataType, Size>::cend() const {
    return const_iterator{*this, Size, true};
}
} // namespace brocolio::container
