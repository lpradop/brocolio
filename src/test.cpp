// #include "algorithm.hpp"
#include "array.hpp"
// #include "matrix.hpp"
// #include "priority_queue.hpp"
// #include <immintrin.h>
// #include "binary_search_tree.hpp"
// #include "graph.hpp"
// #include "stack.hpp"
#include "caesar_cipher.hpp"
#include <iostream>
// #include "unordered_pair.hpp"
// #include "dynamic_matrix.hpp"
// #include "linked_list.hpp"
// using namespace brocolio::container;
using namespace brocolio::crypthography;
// using namespace brocolio::algorithm;
int main() {
  caesar_cipher<97, 122> caesar{};
  int shift{2};
  caesar.encrypt_file("../src/input.txt", "../src/encrypted.txt", shift);
  caesar.decrypt_file("../src/encrypted.txt", "../src/decrypted.txt", shift);
  auto arr{caesar.brute_force_decrypt_file(
      "../src/encrypted.txt", "../src/brute.txt", "../src/words.txt")};
  // dynamic_matrix<int> m{{10, 4}};

  // dynamic_matrix<int> n{std::move(m)};
  // n = n + n;
  // std::cout << n(0, 0) << std::endl;
  // std::cout << n(0, 1) << std::endl;
}
