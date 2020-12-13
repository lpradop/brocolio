namespace brocolio::simd {
template <typename T>

struct block_size {};

template <>
struct block_size<int> {
  static unsigned int constexpr value{8};
};

template <>
struct block_size<short> {
  static unsigned int constexpr value{16};
};

template <>
struct block_size<float> {
  static unsigned int constexpr value{8};
};

template <>
struct block_size<double> {
  static unsigned int constexpr value{4};
};

template <typename T>
unsigned int constexpr block_size_v{block_size<T>::value};
} // namespace brocolio::simd