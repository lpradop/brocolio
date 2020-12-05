#pragma once
#include "array.hpp"
#include <cstdlib>
#include <ctime>
namespace brocolio {
namespace random {

// Brocolio de la CHUNSA library for pseudo random generator, early stage
template <class Class, std::size_t Size>
const Class &box_picker(const container::array<Class, Size> &classes,
                  container::array<std::size_t, Size> frequency) {
  // not safe
  std::srand(time(nullptr));

}

} // namespace random
} // namespace brocolio
