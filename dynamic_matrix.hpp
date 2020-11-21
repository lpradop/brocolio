#pragma once
#include "algorithm.hpp"
#include "ordered_pair.hpp"
#include <immintrin.h>
#include <iostream>
#include <stdexcept>
namespace brocolio::container {

// Dynamic_Matrix template prototype by brocolio de la CHUNSA
template <class DataType> class dynamic_matrix {
public:
  dynamic_matrix() = default;
  dynamic_matrix(std::size_t const rows, std::size_t const columns);
  dynamic_matrix(dynamic_matrix const&);
  dynamic_matrix(dynamic_matrix&&);
  dynamic_matrix(
      std::initializer_list<std::initializer_list<DataType>> const il);
  ~dynamic_matrix();

  dynamic_matrix& operator=(dynamic_matrix const&);
  dynamic_matrix& operator=(dynamic_matrix&&);
  dynamic_matrix<float>& operator+=(dynamic_matrix<float> const& rhs); // TODO
  dynamic_matrix<float> operator+(dynamic_matrix<float> const& rhs) const;
  DataType& operator()(std::size_t const i, std::size_t const j);

  ordered_pair<std::size_t, std::size_t> size() const { return size_; }
  void resize(std::size_t const rows, std::size_t const columns); // TODO

private:
  ordered_pair<std::size_t, std::size_t> size_{0, 0};
  std::size_t matrix_data_size_{0};
  DataType* matrix_data_{nullptr};
};

template <class DataType>
dynamic_matrix<DataType>::dynamic_matrix(std::size_t const rows,
                                         std::size_t const columns)
    : size_(ordered_pair<std::size_t, std::size_t>{rows, columns}),
      matrix_data_size_(size_.x * size_.y),
      matrix_data_(new DataType[matrix_data_size_]) {}

template <class DataType>
dynamic_matrix<DataType>::dynamic_matrix(dynamic_matrix const& other)
    : dynamic_matrix(other.size_.x, other.size_.y) {
  for (std::size_t i{0}; i < matrix_data_size_; ++i)
    matrix_data_[i] = other.matrix_data_[i];
}

template <class DataType>
dynamic_matrix<DataType>::dynamic_matrix(dynamic_matrix<DataType>&& other)
    : size_(other.size_), matrix_data_size_(other.matrix_data_size_) {
  matrix_data_ = other.matrix_data_;
  other.matrix_data_ = nullptr;
}

template <class DataType>
dynamic_matrix<DataType>::dynamic_matrix(
    std::initializer_list<std::initializer_list<DataType>> const il) {
  if (algorithm::all_of(il.begin(), il.end(),
                        [&il](std::initializer_list<DataType> const& x) {
                          return x.size() == il.begin()->size();
                        })) {
    size_ = ordered_pair<std::size_t, std::size_t>{(il.size()),
                                                   (il.begin()->size())};
    matrix_data_size_ = size_.x * size_.y;
    matrix_data_ = new DataType[matrix_data_size_];
    std::size_t i{0};
    for (auto const& row : il) {
      std::size_t j{0};
      for (DataType const& data : row) {
        matrix_data_[size_.x * i + j] = data;
        ++j;
      }
      ++i;
    }
  } else {
    throw std::length_error{
        "invalid size of initializer list on dynamic_matrix creation"};
  }
}

template <class DataType>
dynamic_matrix<DataType>&
dynamic_matrix<DataType>::operator=(dynamic_matrix const& other) {
  if (matrix_data_size_ == other.matrix_data_size_) {
    for (std::size_t i{0}; i < matrix_data_size_; ++i)
      matrix_data_[i] = other.matrix_data_[i];
    return *this;
  } else {
    throw std::length_error{"cannot copy the given matrix"};
  }
}

template <class DataType>
dynamic_matrix<DataType>&
dynamic_matrix<DataType>::operator=(dynamic_matrix&& other) {
  size_ = other.size_;
  matrix_data_size_ = other.matrix_data_size_;
  matrix_data_ = other.matrix_data_;
  other.size_.x = 0;
  other.size_.y = 0;
  other.matrix_data_size_ = 0;
  other.matrix_data_ = nullptr;
  return *this;
}

template <class DataType>
DataType& dynamic_matrix<DataType>::operator()(std::size_t const i,
                                               std::size_t const j) {
  return matrix_data_[size_.x * i + j];
}

template <class DataType>
dynamic_matrix<float>&
dynamic_matrix<DataType>::operator+=(dynamic_matrix<float> const& rhs) {

  if (size_ == rhs.size_) {
#if defined __linux__
#if defined __AVX2__
    std::size_t constexpr block_size{8};
    // __m256i mask{_mm256_set1_epi32(-1)};
#elif defined __AVX__
    std::size_t constexpr block_size{4};
#endif // __AVX2__

    std::size_t const partial_bound{matrix_data_size_ / block_size};
    short const remainder_bound{
        static_cast<short>(matrix_data_size_ % block_size)};
    int arr_mask[block_size]{};
    float* block_lhs_pointer{matrix_data_};
    float* block_rhs_pointer{rhs.matrix_data_};

#if defined __AVX2__
    for (std::size_t i{0}; i < partial_bound; ++i) {
      __m256 simd_lhs{_mm256_loadu_ps(block_lhs_pointer)};
      __m256 simd_rhs{_mm256_loadu_ps(block_rhs_pointer)};
      simd_lhs = _mm256_add_ps(simd_lhs, simd_rhs);
      _mm256_storeu_ps(block_lhs_pointer, simd_lhs);
      block_lhs_pointer += block_size;
      block_rhs_pointer += block_size;
    }

    if (remainder_bound != 0) {
      for (short i{0}; i < remainder_bound; ++i) {
        arr_mask[i] = -1;
      }
      __m256i mask{_mm256_loadu_si256(reinterpret_cast<__m256i_u*>(arr_mask))};
      __m256 simd_lhs{_mm256_maskload_ps(block_lhs_pointer, mask)};
      __m256 simd_rhs{_mm256_maskload_ps(block_rhs_pointer, mask)};
      simd_lhs = _mm256_add_ps(simd_lhs, simd_rhs);
      _mm256_maskstore_ps(block_lhs_pointer, mask, simd_lhs);
    }
#elif defined __AVX__
    // AVX code
#endif // __AVX2__
#endif // __linux__

  } else {
    throw std::length_error{"gaaa"};
  }
  return *this;
}

template <class DataType> // FIXME
dynamic_matrix<float>
dynamic_matrix<DataType>::operator+(dynamic_matrix<float> const& rhs) const {
  dynamic_matrix<float> result{};
  result += rhs;
  result += *this;
  return result;
}

template <class DataType> dynamic_matrix<DataType>::~dynamic_matrix() {
  delete[] matrix_data_;
}
} // namespace brocolio::container
