#pragma once
#include <initializer_list>
#include <iostream>
//prototipo de clase, quiza presente bugs, lo ire mejorando poco a poco.
//TODO implementar destructor
template <class DataType, int size> class SquareMatrix {
private:
  DataType **matrix_;
  static DataType ProductElement(const SquareMatrix<DataType, size> &a,
                                 const SquareMatrix<DataType, size> &b, int i,
                                 int j);

public:
  SquareMatrix(std::initializer_list<std::initializer_list<DataType>> matrix);
  SquareMatrix(const SquareMatrix<DataType, size> &other);
  SquareMatrix() = delete;
  ~SquareMatrix();

  void Print();
  SquareMatrix<DataType, size> &
  operator*=(const SquareMatrix<DataType, size> &rhs) {
    auto a = *this;
    auto b = rhs;
    for (int i = 0; i < size; ++i) {
      for (int j = 0; j < size; ++j) {
        this->matrix_[i][j] = SquareMatrix::ProductElement(a, b, i, j);
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

  DataType *operator[](int i) const {
    if (i > size) {
      std::cout << "Index out of bounds" << std::endl;
      return matrix_[i];
    }

    return matrix_[i];
  }
};

template <class DataType, int size>
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

template <class DataType, int size>
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

template <class DataType, int size>
DataType SquareMatrix<DataType, size>::ProductElement(
    const SquareMatrix<DataType, size> &a,
    const SquareMatrix<DataType, size> &b, int i, int j) {

  DataType element{};
  for (int t = 0; t < size; ++t)
    element += a[i][t] * b[t][j];

  return element;
}
template <class DataType, int size> void SquareMatrix<DataType, size>::Print() {
  for (int row = 0; row < size; ++row) {
    for (int column = 0; column < size; ++column)
      std::cout << matrix_[row][column] << " ";
    std::cout << std::endl;
  }
}
template <class DataType, int size>
SquareMatrix<DataType, size>::~SquareMatrix() {}
