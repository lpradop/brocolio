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
// support for int32 and float types for now
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
  dynamic_matrix& operator+=(dynamic_matrix const& rhs);
  dynamic_matrix& operator*=(dynamic_matrix const& rhs);
  DataType& operator()(SizeType i, SizeType j) const;

  ordered_pair<SizeType, SizeType> size() const noexcept { return size_; }
  SizeType rows() const noexcept { return size_.x; };
  SizeType columns() const noexcept { return size_.y; };

  void print() const noexcept;
  void resize(SizeType rows, SizeType columns); // TODO
  dynamic_matrix transpose() const noexcept;    // todo

private:
  static SizeType constexpr block_size_{simd::block_size_v<DataType>};
  ordered_pair<SizeType, SizeType> size_{0, 0};
  SizeType matrix_data_size_{0};
  DataType* matrix_data_{nullptr};

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
  for (SizeType i{0}; i < matrix_data_size_; ++i) {
    matrix_data_[i] = other.matrix_data_[i];
  }
}

template <concepts::numeric DataType, concepts::numeric SizeType>
dynamic_matrix<DataType, SizeType>::dynamic_matrix(
    dynamic_matrix&& other) noexcept
    : size_{other.size_}, matrix_data_size_{other.matrix_data_size_} {
  delete[] matrix_data_;
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
dynamic_matrix<DataType, SizeType>&
dynamic_matrix<DataType, SizeType>::operator+=(dynamic_matrix const& rhs) {

  if (size_ == rhs.size_) {

    SizeType const partial_bound{matrix_data_size_ / block_size_};
    SizeType const remainder_bound{matrix_data_size_ % block_size_};
    int arr_mask[block_size_]{};
    auto block_lhs_pointer{matrix_data_};
    auto block_rhs_pointer{rhs.matrix_data_};

    for (SizeType i{0}; i < remainder_bound; ++i) {
      arr_mask[i] = -1;
    }

    for (SizeType i{0}; i < partial_bound; ++i) {

      if constexpr (std::is_same_v<DataType, float>) {
        auto simd_lhs{_mm256_loadu_ps(block_lhs_pointer)};
        auto simd_rhs{_mm256_loadu_ps(block_rhs_pointer)};
        simd_lhs = _mm256_add_ps(simd_lhs, simd_rhs);
        _mm256_storeu_ps(block_lhs_pointer, simd_lhs);

      } else if (std::is_same_v<DataType, int>) {
        auto simd_lhs{_mm256_loadu_si256(
            reinterpret_cast<__m256i_u*>(block_lhs_pointer))};
        auto simd_rhs{_mm256_loadu_si256(
            reinterpret_cast<__m256i_u*>(block_rhs_pointer))};

        simd_lhs = _mm256_add_epi32(simd_lhs, simd_rhs);
        _mm256_storeu_si256(reinterpret_cast<__m256i_u*>(block_rhs_pointer),
                            simd_lhs);
      }

      block_lhs_pointer += block_size_;
      block_rhs_pointer += block_size_;
    }

    if (remainder_bound != 0) {
      auto mask{_mm256_loadu_si256(reinterpret_cast<__m256i_u*>(arr_mask))};

      if constexpr (std::is_same_v<DataType, float>) {
        auto simd_lhs{_mm256_maskload_ps(block_lhs_pointer, mask)};
        auto simd_rhs{_mm256_maskload_ps(block_rhs_pointer, mask)};
        simd_lhs = _mm256_add_ps(simd_lhs, simd_rhs);
        _mm256_maskstore_ps(block_lhs_pointer, mask, simd_lhs);

      } else if (std::is_same_v<DataType, int>) {
        auto simd_lhs{_mm256_maskload_epi32(block_lhs_pointer, mask)};
        auto simd_rhs{_mm256_maskload_epi32(block_rhs_pointer, mask)};
        simd_lhs = _mm256_add_epi32(simd_lhs, simd_rhs);
        _mm256_maskstore_epi32(block_lhs_pointer, mask, simd_lhs);
      }
    }

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

template <concepts::numeric DataType, concepts::numeric SizeType>
dynamic_matrix<DataType, SizeType>&
dynamic_matrix<DataType, SizeType>::operator*=(dynamic_matrix const& rhs) {
  if (size_.y == rhs.size_.x) {
    // aliases
    auto const& nrows_of_lhs{size_.x};
    auto const& nrows_of_rhs{rhs.size_.x};
    auto const& ncolumns_of_rhs{rhs.size_.y};

    // new matrix creation

    SizeType const new_matrix_data_size{nrows_of_lhs * ncolumns_of_rhs};
    auto const result{new DataType[new_matrix_data_size]{}};

    // transversal pointers
    auto rhs_pointer{rhs.matrix_data_};
    auto result_pointer{result};

    // product computation
    for (SizeType i{0}; i < nrows_of_rhs; ++i) {
      for (SizeType k{0}; k < nrows_of_rhs; ++k) {
        product_helper(matrix_data_[i * nrows_of_rhs + k], rhs_pointer,
                       result_pointer, ncolumns_of_rhs);
        rhs_pointer += nrows_of_rhs;
      }
      rhs_pointer = rhs.matrix_data_;
      result_pointer += nrows_of_rhs;
    }

    // cleaning and updating matrix
    delete[] matrix_data_;
    matrix_data_ = result;
    size_.x = nrows_of_lhs;
    size_.y = ncolumns_of_rhs;
    matrix_data_size_ = new_matrix_data_size;

    return *this;
  } else {
    throw std::length_error{
        "matrices cannot be multiplied, incompatible sizes"};
  }
}

// calculate scalar*[......] (row vector of b) and accumulate the result into c
template <concepts::numeric DataType, concepts::numeric SizeType>
void dynamic_matrix<DataType, SizeType>::product_helper(
    DataType const scalar, DataType* const b, DataType* const c,
    SizeType const ncolumn_of_b) noexcept {
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
      auto mask{_mm256_loadu_si256(reinterpret_cast<__m256i_u*>(arr_mask))};
      auto b_block{_mm256_maskload_ps(b_block_pointer, mask)};
      auto c_block{_mm256_maskload_ps(c_block_pointer, mask)};
      c_block = _mm256_fmadd_ps(scalar_block, b_block, c_block);
      _mm256_maskstore_ps(c_block_pointer, mask, c_block);
    }
  } else if (std::is_same_v<DataType, int>) {

    auto scalar_block{_mm256_set1_epi32(scalar)};

    for (SizeType i{0}; i < partial_bound; ++i) {
      auto b_block{
          _mm256_loadu_si256(reinterpret_cast<__m256i_u*>(b_block_pointer))};
      auto c_block{
          _mm256_loadu_si256(reinterpret_cast<__m256i_u*>(c_block_pointer))};
      c_block =
          _mm256_add_epi32(_mm256_mullo_epi32(scalar_block, b_block), c_block);

      _mm256_storeu_si256(reinterpret_cast<__m256i_u*>(c_block_pointer),
                          c_block);
      b_block_pointer += block_size_;
      c_block_pointer += block_size_;
    }

    if (remainder_bound != 0) {
      auto mask{_mm256_loadu_si256(reinterpret_cast<__m256i_u*>(arr_mask))};
      auto b_block{_mm256_maskload_epi32(b_block_pointer, mask)};
      auto c_block{_mm256_maskload_epi32(c_block_pointer, mask)};
      c_block =
          _mm256_add_epi32(_mm256_mullo_epi32(scalar_block, b_block), c_block);

      _mm256_maskstore_epi32(c_block_pointer, mask, c_block);
    }
  }
}

// row major order print
template <concepts::numeric DataType, concepts::numeric SizeType>
void dynamic_matrix<DataType, SizeType>::print() const noexcept {
  for (SizeType i{0}; i < matrix_data_size_; ++i) {
    if (i % size_.y == 0) {
      std::cout << "row: " << i / size_.y << std::endl;
    }

    std::cout << matrix_data_[i] << std::endl;
  }
}
} // namespace brocolio::container
