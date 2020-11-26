// #include "algorithm.hpp"
// #include "array.hpp"
// #include "matrix.hpp"
// #include "priority_queue.hpp"
// #include <immintrin.h>
// #include "binary_search_tree.hpp"
// #include "graph.hpp"
// #include "stack.hpp"
#include <iostream>
// #include "unordered_pair.hpp"
#include "dynamic_matrix.hpp"
#include "linked_list.hpp"
using namespace brocolio::container;
// using namespace brocolio::algorithm;
int main() {
  dynamic_matrix<int> m{{10, 4}};
  dynamic_matrix<int> n{std::move(m)};

  n = n + n;
  std::cout << n(0, 0) << std::endl;
  std::cout << n(0, 1) << std::endl;
}
