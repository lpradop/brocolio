#include <iostream>
#include <type_traits>
template <typename T, typename Enable = void> class base_class {};

// my favourite type :D
template <typename T>
class base_class<T, std::enable_if_t<std::is_same_v<T, int>, void>> {
public:
  int some_variable;
};

// not my favourite type :(
// template <typename T>
// class base_class<T, std::enable_if_t<!std::is_same_v<T, int>, void>> {
// public:
// no variable
// };

template <typename T> class derived_class : public base_class<T> {
public:
  // do stuff
  template <typename S = T, typename = std::enable_if_t<std::is_same_v<S, int>>>
  int foo() {
    // base_class<int>::some_variable = 2;
    return 1;
  }

  template <typename S = T,
            typename = std::enable_if_t<std::is_same_v<S, double>>>
  int ga() {
    double a = 2;
    return 1;
  }
};

using namespace std;
template <typename T> struct test {
  template <typename S = T> enable_if_t<is_integral_v<S>, int> a() { return 0; }

  template <typename S = T> enable_if_t<is_floating_point_v<S>, double> b();
};

template <typename T>
template <typename S>
enable_if_t<is_floating_point_v<S>, double> test<T>::b() {
  return 1.0;
}

int main() {
  // outer<int>::inner<double> a;

  std::cout << "gaaaaa" << std::endl;
  std::cout << "gaaaaa" << std::endl;
  std::cout << "gaaaaa" << std::endl;
  std::cout << "gaaaaa" << std::endl;
}
