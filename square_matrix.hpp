#pragma once
#include <initializer_list>
#include <iostream>

namespace brocolio {
namespace container {

// SquareMatrix template by brocolio de la CHUNSA
// TODO add data verification on constructor
template <class DataType, std::size_t size> class square_matrix {
private:
  DataType **matrix_{nullptr};

  static DataType product_element(const square_matrix<DataType, size> &a,
                                  const square_matrix<DataType, size> &b,
                                  const int i, const int j);

public:
  square_matrix(std::initializer_list<std::initializer_list<DataType>> matrix);
  square_matrix(const square_matrix<DataType, size> &other);
  square_matrix() = delete;
  ~square_matrix();
  void print() const;

  square_matrix<DataType, size> &
  operator*=(const square_matrix<DataType, size> &rhs) {
    auto a = *this;
    auto b = rhs;
    for (int i = 0; i < size; ++i) {
      for (int j = 0; j < size; ++j) {
        this->matrix_[i][j] = square_matrix::product_element(a, b, i, j);
      }
    }
    return *this;
  }

  friend square_matrix<DataType, size>
  operator*(square_matrix<DataType, size> lhs,
            const square_matrix<DataType, size> &rhs) {
    lhs *= rhs;
    return lhs;
  }

  DataType *operator[](const int index) const {
    if (index > size or index < 0) {
      std::cout << "index out of bounds" << std::endl;
      return DataType{};
    }
    return matrix_[index];
  }
};

template <class DataType, std::size_t size>
square_matrix<DataType, size>::square_matrix(
    std::initializer_list<std::initializer_list<DataType>> matrix) {
  matrix_ = new DataType *[size];
  for (int i = 0; i < size; ++i) {
    matrix_[i] = new DataType[size];
  }
  int i = 0;
  for (auto &row : matrix) {
    int j = 0;
    for (auto &data : row) {
      matrix_[i][j] = data;
      ++j;
    }
    ++i;
  }
}

template <class DataType, std::size_t size>
square_matrix<DataType, size>::square_matrix(
    const square_matrix<DataType, size> &other) {
  matrix_ = new DataType *[size];
  for (int i = 0; i < size; ++i) {
    matrix_[i] = new DataType[size];
  }

  for (int i = 0; i < size; ++i) {
    for (int j = 0; j < size; ++j) {
      matrix_[i][j] = other.matrix_[i][j];
    }
  }
}

template <class DataType, std::size_t size>
DataType square_matrix<DataType, size>::product_element(
    const square_matrix<DataType, size> &a,
    const square_matrix<DataType, size> &b, const int i, const int j) {
  DataType element{};
  for (std::size_t t = 0; t < size; ++t)
    element += a[i][t] * b[t][j];
  return element;
}

template <class DataType, std::size_t size>
void square_matrix<DataType, size>::print() const {
  for (int row = 0; row < size; ++row) {
    for (int column = 0; column < size; ++column)
      std::cout << matrix_[row][column] << " ";
    std::cout << std::endl;
  }
}

template <class DataType, std::size_t size>
square_matrix<DataType, size>::~square_matrix() {
  for (std::size_t i; i < size; ++i) {
    delete[] matrix_[i];
  }
  delete[] matrix_;
}
} // namespace container
} // namespace brocolio
