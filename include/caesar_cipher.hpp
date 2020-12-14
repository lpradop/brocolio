#pragma once
#include "array.hpp"
#include <cmath>
#include <fstream>
#include <functional>
#include <iostream>
#include <stdexcept>
#include <string>
namespace brocolio::crypthography {
template <int LowerBound, int UpperBound>
class caesar_cipher {
public:
  static_assert(LowerBound < UpperBound && LowerBound >= 0 && UpperBound >= 0,
                "UpperBound must be greater than LowerBound");

  static int constexpr range_size{UpperBound - LowerBound + 1};
  caesar_cipher() = default;
  caesar_cipher(caesar_cipher const&) = default;
  caesar_cipher(caesar_cipher&&) = default;
  ~caesar_cipher() = default;

  void encrypt_file(std::string input_filename, std::string output_filename,
                    int shift) const;
  void decrypt_file(std::string input_filename, std::string output_filename,
                    int shift) const;

  container::array<int, UpperBound - LowerBound + 1>
  brute_force_decrypt_file(std::string input_filename,
                           std::string output_filename,
                           std::string dict_filename) const;

  template <typename... Args>
  void frequency_based_decrypt_file(std::string input_filename,
                                    std::string output_filename,
                                    std::string sample_filename,
                                    Args... sample_filenames) const;

  template <typename... Args>
  container::array<double, UpperBound - LowerBound + 1>
  generate_character_frequency(std::string filename, Args... filenames) const;

private:
  int cipher_number(int x, int shift) const;
  std::string const tmp_filename{"../src/tmp.txt"};

  template <typename... Args>
  void merge_files(std::string filename, Args...) const;

  void merge_files() const;

  void compute_relative_frequency(
      container::array<double, range_size>& frequencies) const noexcept;

  double relative_frequency_diff(
      container::array<double, range_size> const& a,
      container::array<double, range_size> const& b) const noexcept;
};

template <int LowerBound, int UpperBound>
int caesar_cipher<LowerBound, UpperBound>::cipher_number(int x,
                                                         int shift) const {
  if (LowerBound <= x && x <= UpperBound) {
    return ((x + shift - LowerBound) % range_size + range_size) % range_size +
           LowerBound;

  } else {
    throw std::domain_error{"invalid x argument"};
  }
}

template <int LowerBound, int UpperBound>
void caesar_cipher<LowerBound, UpperBound>::encrypt_file(
    std::string input_filename, std::string output_filename, int shift) const {
  std::ifstream input_file{input_filename};
  std::ofstream output_file{output_filename};

  for (int ch{}; ch >= 0;) {
    ch = input_file.get();
    if (ch >= 0 && LowerBound <= ch && ch <= UpperBound) {
      output_file << static_cast<char>(cipher_number(ch, shift));
    } else if (ch >= 0) {
      output_file << static_cast<char>(ch);
    }
  }
}

template <int LowerBound, int UpperBound>
void caesar_cipher<LowerBound, UpperBound>::decrypt_file(
    std::string input_filename, std::string output_filename, int shift) const {
  encrypt_file(input_filename, output_filename, -shift);
}

template <int LowerBound, int UpperBound>
container::array<int, UpperBound - LowerBound + 1>
caesar_cipher<LowerBound, UpperBound>::brute_force_decrypt_file(
    std::string input_filename, std::string output_filename,
    std::string dict_filename) const {
  int possible_shift{0};
  int max_value{0};
  container::array<int, range_size> matches{};
  for (int shift{0}; shift < range_size; ++shift) {
    decrypt_file(input_filename, output_filename, shift);
    std::ifstream decrypted_file{output_filename};
    std::ifstream dict_file{dict_filename};

    for (std::string decrypted_word{}; decrypted_file >> decrypted_word;) {
      for (std::string dict_word{}; dict_file >> dict_word;) {
        if (decrypted_word == dict_word) {
          ++matches[shift];
        }
      }
    }
    if (matches[shift] > max_value) {
      max_value = matches[shift];
      possible_shift = shift;
    }
    decrypted_file.close();
    dict_file.close();
  }
  decrypt_file(input_filename, output_filename, possible_shift);
  return std::move(matches);
}

template <int LowerBound, int UpperBound>
template <typename... Args>
container::array<double, UpperBound - LowerBound + 1>
caesar_cipher<LowerBound, UpperBound>::generate_character_frequency(
    std::string filename, Args... filenames) const {

  std::ofstream tmp_file{tmp_filename, std::ios::trunc};
  tmp_file.close();

  std::ifstream merged_file{tmp_filename};
  merge_files(filename, filenames...);
  container::array<double, range_size> frequencies{};

  for (int ch{}; ch >= 0;) {
    ch = merged_file.get();
    if (ch >= 0 && LowerBound <= ch && ch <= UpperBound) {
      ++frequencies[static_cast<std::size_t>(ch - LowerBound)];
    }
  }

  return std::move(frequencies);
}

template <int LowerBound, int UpperBound>
template <typename... Args>
void caesar_cipher<LowerBound, UpperBound>::frequency_based_decrypt_file(
    std::string input_filename, std::string output_filename,
    std::string sample_filename, Args... sample_filenames) const {

  auto sample_frequencies{
      generate_character_frequency(sample_filename, sample_filenames...)};
  compute_relative_frequency(sample_frequencies);

  double min_diff{2.0};
  container::array<double, range_size> frequency_diffs{};
  for (int i{0}; i < range_size; ++i) {
    decrypt_file(input_filename, output_filename, i);
    auto tmp_frequencies{generate_character_frequency(output_filename)};
    compute_relative_frequency(tmp_frequencies);

    frequency_diffs[i] =
        relative_frequency_diff(sample_frequencies, tmp_frequencies);
    min_diff = frequency_diffs[i] <= min_diff ? frequency_diffs[i] : min_diff;
  }

  for (int i{0}; i < range_size; ++i) {
    if (min_diff == frequency_diffs[i]) {
      decrypt_file(input_filename, output_filename, i);
      break;
    }
  }
}

template <int LowerBound, int UpperBound>
template <typename... Args>
void caesar_cipher<LowerBound, UpperBound>::merge_files(
    std::string const filename, Args... filenames) const {
  std::ifstream file{filename, std::ios_base::binary};
  std::ofstream merged_file{tmp_filename, std::ios_base::app};
  merged_file << file.rdbuf() << std::endl;
  merge_files(filenames...);
}

template <int LowerBound, int UpperBound>
void caesar_cipher<LowerBound, UpperBound>::merge_files() const {}

template <int LowerBound, int UpperBound>
void caesar_cipher<LowerBound, UpperBound>::compute_relative_frequency(
    container::array<double, range_size>& frequencies) const noexcept {
  double sample_size{0};
  for (auto const freq : frequencies) {
    sample_size += freq;
  }
  for (auto& freq : frequencies) {
    freq /= sample_size;
  }
}

template <int LowerBound, int UpperBound>
double caesar_cipher<LowerBound, UpperBound>::relative_frequency_diff(
    container::array<double, range_size> const& a,
    container::array<double, range_size> const& b) const noexcept {
  double frequency_diff{0};
  for (int i{0}; i < range_size; ++i) {
    frequency_diff += std::abs(a[i] - b[i]);
  }
  return frequency_diff;
}

} // namespace brocolio::crypthography