#pragma once
#include <cstddef>
namespace brocolio::memory {
template <typename T>
struct default_delete {
  void operator()(T* pointer) const noexcept { delete pointer; }
  void operator()(std::nullptr_t) const noexcept {}
};

template <typename T>
struct default_delete<T[]> {
  void operator()(T* pointer) const noexcept { delete[] pointer; }
  void operator()(std::nullptr_t) const noexcept {}
};
} // namespace brocolio::memory