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

using namespace brocolio::container;
// using namespace brocolio::algorithm;
int main() {
  dynamic_matrix<int> m{{1,4}};
  dynamic_matrix<int> n{m};
  n(0,0)=0;
  std::cout << m.size().y << std::endl;
  std::cout << m(0,0) << std::endl;
  
}
