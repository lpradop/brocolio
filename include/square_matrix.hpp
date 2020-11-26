#pragma once
#include "algorithm.hpp"
#include "array.hpp"

namespace brocolio {
namespace container {

// SquareMatrix template by brocolio de la CHUNSA
// add exceptions, complete refactor using array
template <class DataType, std::size_t matrix_size> class square_matrix {
public:
  class iterator;
  class const_iterator;
  square_matrix()
      : matrix_(new array<array<DataType, matrix_size>, matrix_size>{}) {}
  square_matrix(const square_matrix &);
  square_matrix(square_matrix &&);
  square_matrix(
      const std::initializer_list<std::initializer_list<DataType>> il);
  ~square_matrix();
  square_matrix &operator=(const square_matrix &);
  square_matrix &operator=(square_matrix &&);
  constexpr std::size_t size() const { return matrix_size; };

  square_matrix &operator*=(const square_matrix<DataType, matrix_size> &rhs);
  array<DataType, matrix_size> &operator[](const std::size_t index);


private:
  array<array<DataType, matrix_size>, matrix_size> *matrix_{nullptr};
  static DataType product_element(const square_matrix<DataType, matrix_size> &a,
                                  const square_matrix<DataType, matrix_size> &b,
                                  const int i, const int j);
};

template <class DataType, std::size_t matrix_size>
square_matrix<DataType, matrix_size>::square_matrix(
    const square_matrix<DataType, matrix_size> &other)
    : matrix_(new array<array<DataType, matrix_size>, matrix_size>{}) {
  *matrix_ = *(other.matrix_);
}

template <class DataType, std::size_t matrix_size>
square_matrix<DataType, matrix_size>::square_matrix(
    square_matrix<DataType, matrix_size> &&other) {
  matrix_ = other.matrix_;
  other.matrix_ = nullptr;
}

template <class DataType, std::size_t matrix_size>
square_matrix<DataType, matrix_size>::square_matrix(
    const std::initializer_list<std::initializer_list<DataType>> il) {
  if (il.size() == matrix_size and
      algorithm::all_of(il.begin(), il.end(),
                        [](const std::initializer_list<DataType> &x) {
                          return x.size() == matrix_size;
                        })) {
    matrix_ = new array<array<DataType, matrix_size>, matrix_size>{};
    std::size_t i = 0;
    for (auto &row : il) {
      std::size_t j = 0;
      for (auto &data : row) {
        (*matrix_)[i][j] = data;
        ++j;
      }
      ++i;
    }
  } else {
    throw std::length_error{
        "invalid size of initializer list on matrix creation"};
  }
}

template <class DataType, std::size_t matrix_size>
array<DataType, matrix_size> &
square_matrix<DataType, matrix_size>::operator[](const std::size_t index) {
  if (index < matrix_size) {
    return (*matrix_)[index];
  } else {
    throw std::out_of_range{"trying to access an element out of bounds"};
  }
}

template <class DataType, std::size_t matrix_size>
square_matrix<DataType, matrix_size> &
operator*=(square_matrix<DataType, matrix_size> lhs,
           const square_matrix<DataType, matrix_size> &rhs) {
  lhs *= rhs;
  return lhs;
}

template <class DataType, std::size_t matrix_size>
square_matrix<DataType, matrix_size> &
square_matrix<DataType, matrix_size>::operator*=(
    const square_matrix<DataType, matrix_size> &rhs) {
  auto a = *this;
  auto b = rhs;
  for (int i = 0; i < matrix_size; ++i) {
    for (int j = 0; j < matrix_size; ++j) {
      this->matrix_[i][j] = square_matrix::product_element(a, b, i, j);
    }
  }
  return *this;
}

template <class DataType, std::size_t matrix_size>
DataType square_matrix<DataType, matrix_size>::product_element(
    const square_matrix<DataType, matrix_size> &a,
    const square_matrix<DataType, matrix_size> &b, const int i, const int j) {
  DataType element{};
  for (std::size_t t = 0; t < matrix_size; ++t)
    element += a[i][t] * b[t][j];
  return element;
}

template <class DataType, std::size_t matrix_size>
square_matrix<DataType, matrix_size>::~square_matrix() {
  delete matrix_;
  matrix_ = nullptr;
}
} // namespace container
} // namespace brocolio
