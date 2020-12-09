#pragma once
namespace brocolio::memory {
template <typename T> struct default_delete {
  void operator()(T*& p) { delete p; }
};

template <typename T> struct default_delete<T[]> {
  void operator()(T*& p) { delete[] p; }
};
} // namespace brocolio::memory