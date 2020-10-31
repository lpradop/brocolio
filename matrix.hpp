#pragma once
#include "algorithm.hpp"
#include "array.hpp"
#include "ordered_pair.hpp"

#include <iostream>
namespace brocolio {
namespace container {

// Matrix template prototype by brocolio de la CHUNSA
template <class DataType, std::size_t M, std::size_t N> class matrix {
public:
  static_assert(M > 0 and N > 0, "matrix invalid size");
  class iterator;
  template <std::size_t Size> struct linear_block;
  matrix();
  matrix(const matrix &);
  matrix(matrix &&);
  matrix(const std::initializer_list<std::initializer_list<DataType>> il);
  ~matrix();
  matrix &operator=(const matrix &);          // ni
  matrix &operator=(matrix &&);               // ni
  explicit operator matrix<DataType, N, M>(); // ni

  constexpr ordered_pair<std::size_t, std::size_t> size() const {
    return ordered_pair<std::size_t, std::size_t>{M, N};
  };

  matrix &operator*=(const matrix &rhs); // ni
  linear_block<N> &operator[](const std::size_t i);

private:
  array<DataType, M * N> *matrix_data_{nullptr};
  array<linear_block<N>, M> *rows_{nullptr};
  array<linear_block<M>, N> *columns_{nullptr};
};

template <class DataType, std::size_t M, std::size_t N>
template <std::size_t Size>
struct matrix<DataType, M, N>::linear_block {
  array<DataType *, Size> block{};

  linear_block() = default;
  ~linear_block() = default;
  linear_block(const linear_block &) = default;
  linear_block(linear_block &&) = default;
  DataType &operator[](const std::size_t index) { return *(block[index]); }
};

template <class DataType, std::size_t M, std::size_t N>
matrix<DataType, M, N>::matrix()
    : matrix_data_(new array<DataType, M * N>{}),
      rows_(new array<linear_block<N>, M>{}),
      columns_(new array<linear_block<M>, N>{}) {
  std::size_t i{0};
  while (i < M) {
    std::size_t j{0};
    while (j < N) {
      (*rows_)[i].block[j] = &((*matrix_data_)[M * i + j]);
      (*columns_)[j].block[i] = &((*matrix_data_)[N * j + i]);
      ++j;
    }
    ++i;
  }
}

template <class DataType, std::size_t M, std::size_t N>
matrix<DataType, M, N>::matrix(const matrix<DataType, M, N> &other) : matrix() {
  *matrix_data_ = *(other.matrix_data_);
}

template <class DataType, std::size_t M, std::size_t N>
matrix<DataType, M, N>::matrix(matrix<DataType, M, N> &&other) {
  matrix_data_ = other.matrix_data_;
  rows_ = other.rows_;
  columns_ = other.columns_;
  other.matrix_data_ = nullptr;
  other.rows_ = nullptr;
  other.columns_ = nullptr;
}

template <class DataType, std::size_t M, std::size_t N>
matrix<DataType, M, N>::matrix(
    const std::initializer_list<std::initializer_list<DataType>> il)
    : matrix() {
  if (il.size() == M and
      algorithm::all_of(il.begin(), il.end(),
                        [](const std::initializer_list<DataType> &x) {
                          return x.size() == M;
                        })) {
    std::size_t i = 0;
    for (const auto &row : il) {
      std::size_t j = 0;
      for (const auto &data : row) {
        (*matrix_data_)[M * i + j] = data;
        ++j;
      }
      ++i;
    }
  } else {
    throw std::length_error{
        "invalid size of initializer list on matrix creation"};
  }
}

template <class DataType, std::size_t M, std::size_t N>
typename matrix<DataType, M, N>::template linear_block<N> &
matrix<DataType, M, N>::operator[](const std::size_t i) {
  return (*rows_)[i];
}

// template <class DataType, std::size_t M, std::size_t N>
// matrix<DataType, M, N> &operator*=(matrix<DataType, M, N> lhs,
//                                    const matrix<DataType, M, N> &rhs) {
//   lhs *= rhs;
//   return lhs;
// }

// template <class DataType, std::size_t M, std::size_t N>
// matrix<DataType, M, N> &
// matrix<DataType, M, N>::operator*=(const matrix<DataType, M, N> &rhs) {
//   auto a = *this;
//   auto b = rhs;
//   for (int i = 0; i < M; ++i) {
//     for (int j = 0; j < M; ++j) {
//       this->matrix_[i][j] = matrix::product_element(a, b, i, j);
//     }
//   }
//   return *this;
// }

template <class DataType, std::size_t M, std::size_t N>
matrix<DataType, M, N>::~matrix() {
  delete matrix_data_;
  delete rows_;
  delete columns_;
}
} // namespace container
} // namespace brocolio
