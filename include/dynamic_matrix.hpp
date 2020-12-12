#pragma once
#include "algorithm.hpp"
#include "concepts.hpp"
#include "ordered_pair.hpp"
#include "simd.hpp"
#include <immintrin.h>
#include <iostream>
#include <stdexcept>
namespace brocolio::container {

// Dynamic_Matrix template prototype by brocolio de la CHUNSA
// TODO be able to choose the size type of the matrix
template <concepts::numeric DataType = int,
          concepts::numeric SizeType = unsigned int>
class dynamic_matrix {
public:
  dynamic_matrix() = default;
  dynamic_matrix(SizeType rows, SizeType columns) noexcept;
  dynamic_matrix(dynamic_matrix const&) noexcept;
  dynamic_matrix(dynamic_matrix&&) noexcept;
  dynamic_matrix(std::initializer_list<std::initializer_list<DataType>> il);
  ~dynamic_matrix() noexcept;

  dynamic_matrix& operator=(dynamic_matrix const&) noexcept;
  dynamic_matrix& operator=(dynamic_matrix&&) noexcept;
  dynamic_matrix operator+(dynamic_matrix const& rhs) const;
  dynamic_matrix operator*(dynamic_matrix const& rhs) const;
  dynamic_matrix<float>& operator+=(dynamic_matrix<float> const& rhs);
  dynamic_matrix<int>& operator+=(dynamic_matrix<int> const& rhs);
  dynamic_matrix<float>& operator*=(dynamic_matrix<float> const& rhs);
  DataType& operator()(SizeType i, SizeType j) const;

  ordered_pair<SizeType, SizeType> size() const noexcept { return size_; }
  void resize(SizeType rows, SizeType columns); // TODO
  dynamic_matrix transpose() const noexcept;

private:
  ordered_pair<SizeType, SizeType> size_{0, 0};
  SizeType matrix_data_size_{0};
  DataType* matrix_data_{nullptr};
  static SizeType constexpr block_size_{simd::block_size_v<DataType>};

  DataType* raw_transpose() const noexcept;

  void product_helper(DataType s, DataType* b, DataType* c,
                      SizeType n) noexcept;
};

template <concepts::numeric DataType, concepts::numeric SizeType>
dynamic_matrix<DataType, SizeType>::dynamic_matrix(
    SizeType const rows, SizeType const columns) noexcept
    : size_{ordered_pair<SizeType, SizeType>{rows, columns}},
      matrix_data_size_{size_.x * size_.y},
      matrix_data_{new DataType[matrix_data_size_]{}} {}

template <concepts::numeric DataType, concepts::numeric SizeType>
dynamic_matrix<DataType, SizeType>::dynamic_matrix(
    dynamic_matrix const& other) noexcept
    : dynamic_matrix{other.size_.x, other.size_.y} {
  for (SizeType i{0}; i < matrix_data_size_; ++i)
    matrix_data_[i] = other.matrix_data_[i];
}

template <concepts::numeric DataType, concepts::numeric SizeType>
dynamic_matrix<DataType, SizeType>::dynamic_matrix(
    dynamic_matrix&& other) noexcept
    : size_{other.size_}, matrix_data_size_{other.matrix_data_size_} {
  if (matrix_data_ != nullptr) delete[] matrix_data_;
  matrix_data_ = other.matrix_data_;
  other.matrix_data_ = nullptr;
}

template <concepts::numeric DataType, concepts::numeric SizeType>
dynamic_matrix<DataType, SizeType>::dynamic_matrix(
    std::initializer_list<std::initializer_list<DataType>> const il) {
  if (algorithm::all_of(il.begin(), il.end(),
                        [&il](std::initializer_list<DataType> const& x) {
                          return x.size() == il.begin()->size();
                        })) {
    size_ = ordered_pair<SizeType, SizeType>{
        static_cast<SizeType>(il.size()),
        static_cast<SizeType>(il.begin()->size())};
    matrix_data_size_ = size_.x * size_.y;
    matrix_data_ = new DataType[matrix_data_size_];
    SizeType i{0};
    for (auto const& row : il) {
      SizeType j{0};
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

template <concepts::numeric DataType, concepts::numeric SizeType>
dynamic_matrix<DataType, SizeType>::~dynamic_matrix() noexcept {
  delete[] matrix_data_;
}

template <concepts::numeric DataType, concepts::numeric SizeType>
dynamic_matrix<DataType, SizeType>&
dynamic_matrix<DataType, SizeType>::operator=(
    dynamic_matrix const& other) noexcept {
  if (matrix_data_size_ == other.matrix_data_size_) {
    for (SizeType i{0}; i < matrix_data_size_; ++i)
      matrix_data_[i] = other.matrix_data_[i];
    return *this;
  } else {
    throw std::length_error{"cannot copy the given matrix"};
  }
}

template <concepts::numeric DataType, concepts::numeric SizeType>
dynamic_matrix<DataType, SizeType>&
dynamic_matrix<DataType, SizeType>::operator=(dynamic_matrix&& other) noexcept {
  size_ = other.size_;
  matrix_data_size_ = other.matrix_data_size_;
  matrix_data_ = other.matrix_data_;
  other.size_.x = 0;
  other.size_.y = 0;
  other.matrix_data_size_ = 0;
  other.matrix_data_ = nullptr;
  return *this;
}

template <concepts::numeric DataType, concepts::numeric SizeType>
DataType&
dynamic_matrix<DataType, SizeType>::operator()(SizeType const i,
                                               SizeType const j) const {
  return matrix_data_[size_.x * i + j];
}

template <concepts::numeric DataType, concepts::numeric SizeType>
dynamic_matrix<float>& dynamic_matrix<DataType, SizeType>::operator+=(
    dynamic_matrix<float> const& rhs) {

  if (size_ == rhs.size_) {
#if defined __linux__
#if defined __AVX2__
    SizeType constexpr block_size{8};
#elif defined __AVX__
    SizeType constexpr block_size{4};
#endif // __AVX2__

    SizeType const partial_bound{matrix_data_size_ / block_size};
    short const remainder_bound{
        static_cast<short>(matrix_data_size_ % block_size)};
    int arr_mask[block_size]{};
    float* block_lhs_pointer{matrix_data_};
    float* block_rhs_pointer{rhs.matrix_data_};

#if defined __AVX2__
    for (SizeType i{0}; i < partial_bound; ++i) {
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

template <concepts::numeric DataType, concepts::numeric SizeType>
dynamic_matrix<int>&
dynamic_matrix<DataType, SizeType>::operator+=(dynamic_matrix<int> const& rhs) {

  if (size_ == rhs.size_) {
#if defined __linux__
#if defined __AVX2__
    SizeType constexpr block_size{8};
    // __m256i mask{_mm256_set1_epi32(-1)};
#elif defined __AVX__
    SizeType constexpr block_size{4};
#endif // __AVX2__

    SizeType const partial_bound{matrix_data_size_ / block_size};
    short const remainder_bound{
        static_cast<short>(matrix_data_size_ % block_size)};
    int arr_mask[block_size]{};
    int* block_lhs_pointer{matrix_data_};
    int* block_rhs_pointer{rhs.matrix_data_};

#if defined __AVX2__
    // block addition
    for (SizeType i{0}; i < partial_bound; ++i) {
      // registers creation
      __m256i simd_lhs{
          _mm256_loadu_si256(reinterpret_cast<__m256i_u*>(block_lhs_pointer))};
      __m256i simd_rhs{
          _mm256_loadu_si256(reinterpret_cast<__m256i_u*>(block_rhs_pointer))};

      // addition of registers
      simd_lhs = _mm256_add_epi32(simd_lhs, simd_rhs);

      // store partial result on lhs
      _mm256_storeu_si256(reinterpret_cast<__m256i_u*>(block_rhs_pointer),
                          simd_lhs);

      block_lhs_pointer += block_size;
      block_rhs_pointer += block_size;
    }

    // remainder elements addition
    if (remainder_bound != 0) {
      // setting mask
      for (short i{0}; i < remainder_bound; ++i) {
        arr_mask[i] = -1;
      }
      __m256i mask{_mm256_loadu_si256(reinterpret_cast<__m256i_u*>(arr_mask))};
      // registers creation
      __m256i simd_lhs{_mm256_maskload_epi32(block_lhs_pointer, mask)};
      __m256i simd_rhs{_mm256_maskload_epi32(block_rhs_pointer, mask)};

      // addition of registers
      simd_lhs = _mm256_add_ps(simd_lhs, simd_rhs);

      // store remainder result on lhs
      _mm256_maskstore_epi32(block_lhs_pointer, mask, simd_lhs);
    }

#elif defined __AVX__
    // AVX code
#endif // __AVX2__
#endif // __linux__
    return *this;

  } else {
    throw std::length_error{"matrices cannot be added"};
  }
}

template <concepts::numeric DataType, concepts::numeric SizeType>
dynamic_matrix<DataType, SizeType>
dynamic_matrix<DataType, SizeType>::operator+(dynamic_matrix const& rhs) const {
  if (size_ == rhs.size_) {
    dynamic_matrix<DataType, SizeType> result{size_.x, size_.y};
    result += rhs;
    result += *this;
    return result;
  } else {
    throw std::length_error{"matrices cannot be added, on a+b call"};
  }
}

template <concepts::numeric DataType, concepts::numeric SizeType>
dynamic_matrix<DataType, SizeType>
dynamic_matrix<DataType, SizeType>::operator*(dynamic_matrix const& rhs) const {
  if (size_.y == rhs.size_.x) {
    dynamic_matrix<DataType, SizeType> result{*this};
    result *= rhs;
    return result;
  } else {
    throw std::length_error{"matrices cannot be multiplied"};
  }
}

template <concepts::numeric DataType, concepts::numeric SizeType> // need test
dynamic_matrix<float>& dynamic_matrix<DataType, SizeType>::operator*=(
    dynamic_matrix<float> const& rhs) {

  if (size_.y == rhs.size_.x) {
#if defined __linux__
#if defined __AVX2__
    // aliases
    auto const& nrows_of_lhs{size_.x};
    auto const& nrows_of_rhs{rhs.size_.x};
    auto const& ncolumns_of_rhs{rhs.size_.y};

    // new matrix creation
    SizeType const new_matrix_data_size{nrows_of_lhs * ncolumns_of_rhs};
    auto result{new DataType[new_matrix_data_size]{}};

    // transversal pointers
    for (SizeType i{0}; i < nrows_of_rhs; ++i) {
      for (SizeType k{0}; k < nrows_of_rhs; ++k) {
        product_helper(matrix_data_[i * nrows_of_rhs + k],
                       &rhs.matrix_data_[k * ncolumns_of_rhs],
                       &result[i * nrows_of_rhs], ncolumns_of_rhs);
      }
    }

    // cleaning and updating matrix
    delete[] matrix_data_;
    matrix_data_ = result;
    size_.x = nrows_of_lhs;
    size_.y = ncolumns_of_rhs;
    matrix_data_size_ = new_matrix_data_size;

    return *this;
  } else {
    throw std::length_error{"matrices cannot be multiplied"};
  }
#endif // __AVX2__
#endif // __linux__
}

// calculate scalar*[......] (row vector of b) and accumulate the result into c
template <concepts::numeric DataType, concepts::numeric SizeType>
void dynamic_matrix<DataType, SizeType>::product_helper(
    DataType const scalar, DataType* const b, DataType* const c,
    SizeType const ncolumn_of_b) noexcept {
#if defined __linux__
#if defined __AVX2__
  // variables for block by block computation
  SizeType const partial_bound{ncolumn_of_b / block_size_};
  SizeType const remainder_bound{ncolumn_of_b % block_size_};
  auto b_block_pointer{b};
  auto c_block_pointer{c};

  // mask for computation of remainder elementents
  int arr_mask[block_size_]{};
  for (SizeType i{0}; i < remainder_bound; ++i) {
    arr_mask[i] = -1;
  }

  if constexpr (std::is_same_v<DataType, float>) {

    auto scalar_block{_mm256_set1_ps(scalar)};

    for (SizeType i{0}; i < partial_bound; ++i) {
      auto b_block{_mm256_loadu_ps(b_block_pointer)};
      auto c_block{_mm256_loadu_ps(c_block_pointer)};
      c_block = _mm256_fmadd_ps(scalar_block, b_block, c_block);
      _mm256_storeu_ps(c_block_pointer, c_block);
      b_block_pointer += block_size_;
      c_block_pointer += block_size_;
    }

    if (remainder_bound != 0) {
      __m256i mask{_mm256_loadu_si256(reinterpret_cast<__m256i_u*>(arr_mask))};
      auto b_block{_mm256_maskload_ps(b_block_pointer, mask)};
      auto c_block{_mm256_maskload_ps(c_block_pointer, mask)};
      c_block = _mm256_fmadd_ps(scalar_block, b_block, c_block);
      _mm256_maskstore_ps(c_block_pointer, mask, c_block);
    }

#endif // __AVX2__
#endif // __linux__
  }
}
} // namespace brocolio::container
