#pragma once
#include "array.hpp"
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
namespace brocolio::crypthography {
template <int LowerBound, int UpperBound> class caesar_cipher {
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

private:
  int cipher_number(int x, int shift) const;
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
  input_file.close();
  output_file.close();
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
    if(matches[shift] > max_value ){
      max_value=matches[shift];
      possible_shift=shift;
    }
    decrypted_file.close();
    dict_file.close();
  }
  decrypt_file(input_filename, output_filename, possible_shift);
  return std::move(matches);
}
} // namespace brocolio::crypthography