#include "dynamic_matrix.hpp"
#include <iostream>
#include <limits>
int main() {
  brocolio::container::dynamic_matrix<float> m{{1, 2}};
  brocolio::container::dynamic_matrix<float> n{{1, 4}, {1, 4}};
  std::cout << m.size().x << "," << m.size().y << std::endl;
  m=m * n;
  for (int i{0}; i < m.size().x; ++i) {
    for (int j{0}; j < m.size().y; ++j) {
      std::cout << m(i, j) << std::endl;
    }
  }
  std::cout << m.size().x << "," << m.size().y << std::endl;
}
// #include "caesar_cipher.hpp"
// int main() {
//   brocolio::crypthography::caesar_cipher<97, 122> caesar{};
//   caesar.generate_character_frequency("../src/1.txt", "../src/2.txt",
//                                       "../src/3.txt");
// }