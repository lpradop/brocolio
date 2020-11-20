// #include "algorithm.hpp"
// #include "array.hpp"
// #include "matrix.hpp"
// #include "priority_queue.hpp"
// #include <immintrin.h>
#include "binary_search_tree.hpp"
#include "graph.hpp"
#include "stack.hpp"
#include <iostream>
// #include "unordered_pair.hpp"

using namespace brocolio::container;
// using namespace brocolio::algorithm;
int main() {
  // stack<int> s{};
  // for (int i{0}; i < 4; ++i) {
  //   s.push(i);
  // }
  // while (not s.empty()) {
  //   std::cout << s.pop() << std::endl;
  // }
  graph<int> g{};
  g.insert_vertex(1);
  g.insert_vertex(2);
  g.insert_vertex(3);
  g.insert_edge(1, 2);
  g.insert_edge(3, 2);

  // std::cout <<  << std::endl;
  // std::cout << g.insert_edge(2, 1) << std::endl;

  auto list{g.adyacent_vertices(2)};
  list.print();
  std::cout << list.empty() << std::endl;
  std::cout << list.size() << std::endl;
  g.generate_dot_file("out");
  // binary_search_tree<int> a{10};
  // a.insert(15);
  // a.insert(16);
  // a.insert(11);
  // a.insert(1);
  // a.insert(0);
  // a.insert(20);
  // a.insert(21);
  // a.print(transversal_method::post_order);
  // unordered_pair<int, int> a{2, 5};
  // unordered_pair<int, int> b{50, 2};
  // std::cout << (a==b) << std::endl;

  // matrix<float, 2, 3> m{{2, 0, 0}, {2, 10, 10}};
  // matrix<float, 2, 3> n{{1, 0, 0}, {2, 1, 1}};
  // matrix<float, 2, 3> c{m+n};
  // matrix<int,4,5> k{};
  // c=m+n;
  // for (int i{0}; i < 3; ++i) {
  // }
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
