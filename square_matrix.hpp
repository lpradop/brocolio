#pragma once
#include <initializer_list>
#include <iostream>

// SquareMatrix template by brocolio de la CHUNSA
// TODO add data verification on constructor
template <class DataType, std::size_t size> class SquareMatrix {
private:
  DataType **matrix_ = nullptr;

  static DataType product_element(const SquareMatrix<DataType, size> &a,
                                  const SquareMatrix<DataType, size> &b,
                                  const int i, const int j);

public:
  SquareMatrix(std::initializer_list<std::initializer_list<DataType>> matrix);
  SquareMatrix(const SquareMatrix<DataType, size> &other);
  SquareMatrix() = delete;
  ~SquareMatrix();
  void print() const;

  SquareMatrix<DataType, size> &
  operator*=(const SquareMatrix<DataType, size> &rhs) {
    auto a = *this;
    auto b = rhs;
    for (int i = 0; i < size; ++i) {
      for (int j = 0; j < size; ++j) {
        this->matrix_[i][j] = SquareMatrix::product_element(a, b, i, j);
      }
    }
    return *this;
  }

  friend SquareMatrix<DataType, size>
  operator*(SquareMatrix<DataType, size> lhs,
            const SquareMatrix<DataType, size> &rhs) {
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
SquareMatrix<DataType, size>::SquareMatrix(
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
SquareMatrix<DataType, size>::SquareMatrix(
    const SquareMatrix<DataType, size> &other) {
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
DataType SquareMatrix<DataType, size>::product_element(
    const SquareMatrix<DataType, size> &a,
    const SquareMatrix<DataType, size> &b, const int i, const int j) {
  DataType element{};
  for (std::size_t t = 0; t < size; ++t)
    element += a[i][t] * b[t][j];
  return element;
}

template <class DataType, std::size_t size>
void SquareMatrix<DataType, size>::print() const {
  for (int row = 0; row < size; ++row) {
    for (int column = 0; column < size; ++column)
      std::cout << matrix_[row][column] << " ";
    std::cout << std::endl;
  }
}

template <class DataType, std::size_t size>
SquareMatrix<DataType, size>::~SquareMatrix() {
  for (std::size_t i; i < size; ++i) {
    delete[] matrix_[i];
  }
  delete[] matrix_;
}
