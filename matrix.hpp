#pragma once
#include "algorithm.hpp"
#include "array.hpp"
#include "ordered_pair.hpp"
#include <immintrin.h>
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
  matrix<float, M, M> &operator*=(const matrix<float, M, M> &rhs);
  matrix<float, M, N> &operator+=(const matrix<float, M, N> &rhs);
  DataType &operator()(const std::size_t i, const std::size_t j);

private:
  DataType *matrix_data_{nullptr};
  static constexpr std::size_t size_{M * N};
};

template <class DataType, std::size_t M, std::size_t N>
matrix<DataType, M, N>::matrix() : matrix_data_(new DataType[size_]) {}

template <class DataType, std::size_t M, std::size_t N>
matrix<DataType, M, N>::matrix(const matrix<DataType, M, N> &other) : matrix() {
  // TODO
}

template <class DataType, std::size_t M, std::size_t N>
matrix<DataType, M, N>::matrix(matrix<DataType, M, N> &&other) {
  matrix_data_ = other.matrix_data_;
  other.matrix_data_ = nullptr;
}

template <class DataType, std::size_t M, std::size_t N>
matrix<DataType, M, N>::matrix(
    const std::initializer_list<std::initializer_list<DataType>> il)
    : matrix() {
  if (il.size() == M and
      algorithm::all_of(il.begin(), il.end(),
                        [](const std::initializer_list<DataType> &x) {
                          return x.size() == N;
                        })) {
    std::size_t i{0};
    for (const auto &row : il) {
      std::size_t j{0};
      for (const DataType &data : row) {
        matrix_data_[N * i + j] = data;
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
DataType &matrix<DataType, M, N>::operator()(const std::size_t i,
                                             const std::size_t j) {
  return matrix_data_[N * i + j];
}
// template <class DataType, std::size_t M, std::size_t N>
// matrix<DataType, M, N> &operator*=(matrix<DataType, M, N> lhs,
//                                    const matrix<DataType, M, N> &rhs) {
//   lhs *= rhs;
//   return lhs;
// }

// template <class DataType, std::size_t M, std::size_t N>
// matrix<DataType, M, N> &matrix<DataType, M, N>::operator*=(const matrix &rhs)
// { static_assert(M == N, "matrix must be a square matrix"); return *this;
// }

template <class DataType, std::size_t M, std::size_t N>
matrix<float, M, N> &
matrix<DataType, M, N>::operator+=(const matrix<float, M, N> &rhs) {
#if defined __linux__
#if defined __AVX2__
  constexpr std::size_t block_size{8};
  // __m256i mask{_mm256_set1_epi32(-1)};
#elif defined __AVX__
  constexpr std::size_t block_size{4};
#endif // __AVX2__

  constexpr std::size_t partial_bound{size_ / block_size};
  constexpr short remainder_bound{static_cast<short>(size_ % block_size)};
  int arr_mask[block_size]{};
  float *block_lhs_pointer{matrix_data_};
  float *block_rhs_pointer{rhs.matrix_data_};

#if defined __AVX2__
  for (std::size_t i{0}; i < partial_bound; ++i) {
    __m256 simd_lhs{_mm256_loadu_ps(block_lhs_pointer)};
    __m256 simd_rhs{_mm256_loadu_ps(block_rhs_pointer)};
    simd_lhs = _mm256_add_ps(simd_lhs, simd_rhs);
    _mm256_storeu_ps(block_lhs_pointer, simd_lhs);
    block_lhs_pointer += 8;
    block_rhs_pointer += 8;
  }

  if (remainder_bound != 0) {
    for (short i{0}; i < remainder_bound; ++i) {
      arr_mask[i] = -1;
    }
    __m256i mask{_mm256_loadu_si256(reinterpret_cast<__m256i_u *>(arr_mask))};
    __m256 simd_lhs{_mm256_maskload_ps(block_lhs_pointer, mask)};
    __m256 simd_rhs{_mm256_maskload_ps(block_rhs_pointer, mask)};
    simd_lhs = _mm256_add_ps(simd_lhs, simd_rhs);
    _mm256_maskstore_ps(block_lhs_pointer, mask, simd_lhs);
  }
#elif defined __AVX__
  // AVX code
#endif // __AVX2__
#endif // __linux__
  return *this;
}

template <class DataType, std::size_t M, std::size_t N>
matrix<DataType, M, N>::~matrix() {
  delete[] matrix_data_;
}
} // namespace container
} // namespace brocolio
