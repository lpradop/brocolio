// #include "algorithm.hpp"
// #include "simple_linked_list.hpp"
// #include "array.hpp"
#include "matrix.hpp"
#include "binary_tree.hpp"
// #include "priority_queue.hpp"
#include <immintrin.h>
#include <iostream>
using namespace brocolio::container;
// using namespace brocolio::algorithm;
int main() {
  matrix<float, 2, 3> m{{2, 0, 0}, {2, 10, 10}};
  matrix<float, 2, 3> n{{1, 0, 0}, {2, 1, 1}};
  matrix<float, 2, 3> c{m+n};
  // c=m+n;
  // for (int i{0}; i < 3; ++i) {
    // std::cout << c(1, i) << std::endl;
  // }
  binary_tree<int,binary_tree_variation::STANDARD> s{};
  std::cout << s.insert() << std::endl;
  binary_tree<int,binary_tree_variation::ORDERED> o{};
  std::cout << o.insert() << std::endl;
  // m(1, 0) = 0;
  // std::cout << m(1, 0) << std::endl;
  // matrix<int,2,2>cm{m};

  // for (int i = 0; i < n; ++i) {
  // std::cout << arr[i] << std::endl;
  // }
  // std::cout << b[1] << std::endl;
  // array<int&,6> a{1,2,3,4,5,6};
  // __m256 a=_mm256_set1_ps(5.0);
  // __m256 b=_mm256_set1_ps(1.0);
  // __m256i b=_mm256_set1_epi32(5);
  // a= _mm256_add_ps(a, b);
  // std::cout << a[1] << std::endl;
  // int **p = new int *[4];
  // m[1][0]=1;
  // std::cout << m[0][0] << std::endl;
  // std::cout << cm[1][0] << std::endl;
}
