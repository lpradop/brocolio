#pragma once
#include "default_delete.hpp"
#include <compare>

namespace brocolio::memory {
template <typename Type, typename Deleter = default_delete<Type>>
class unique_ptr {
public:
  unique_ptr() = default;
  unique_ptr(std::nullptr_t) noexcept {};
  explicit unique_ptr(Type*& pointer) noexcept;
  explicit unique_ptr(Type*&& pointer) noexcept;
  unique_ptr(unique_ptr const&) = delete;
  explicit unique_ptr(unique_ptr&&) noexcept;
  ~unique_ptr() noexcept;
  unique_ptr& operator=(unique_ptr const&) = delete;
  unique_ptr& operator=(unique_ptr&&) noexcept;
  Type& operator*() const;
  Type* operator->() const noexcept;
  bool operator==(unique_ptr const&) const noexcept;
  bool operator!=(unique_ptr const&) const noexcept;
  Type* get() const noexcept;
  void reset(Type*& pointer) noexcept;
  void reset(Type*&& pointer) noexcept;

private:
  Type* raw_pointer_{nullptr};
  Deleter deleter_{};
};

template <typename Type, typename Deleter>
unique_ptr<Type, Deleter>::unique_ptr(Type*& pointer) noexcept
    : raw_pointer_{pointer} {
  pointer = nullptr;
}

template <typename Type, typename Deleter>
unique_ptr<Type, Deleter>::unique_ptr(Type*&& pointer) noexcept
    : unique_ptr{pointer} {}

template <typename Type, typename Deleter>
unique_ptr<Type, Deleter>::unique_ptr(unique_ptr&& other) noexcept
    : raw_pointer_(other.raw_pointer_) {
  other.raw_pointer_ = nullptr;
}

template <typename Type, typename Deleter>
unique_ptr<Type, Deleter>::~unique_ptr() noexcept {
  deleter_(raw_pointer_);
}

template <typename Type, typename Deleter>
unique_ptr<Type, Deleter>&
unique_ptr<Type, Deleter>::operator=(unique_ptr&& other) noexcept {
  deleter_(raw_pointer_);
  raw_pointer_ = other.raw_pointer_;
  other.raw_pointer_ = nullptr;
}

template <typename Type, typename Deleter>
Type& unique_ptr<Type, Deleter>::operator*() const {
  return *raw_pointer_;
}

template <typename Type, typename Deleter>
Type* unique_ptr<Type, Deleter>::operator->() const noexcept {
  return raw_pointer_;
}

template <typename Type, typename Deleter>
bool unique_ptr<Type, Deleter>::operator==(
    unique_ptr const& rhs) const noexcept {
  return raw_pointer_ == rhs.raw_pointer_;
}

template <typename Type, typename Deleter>
bool unique_ptr<Type, Deleter>::operator!=(
    unique_ptr const& rhs) const noexcept {
  return !(*this == rhs);
}

template <typename Type, typename Deleter>
Type* unique_ptr<Type, Deleter>::get() const noexcept {
  return raw_pointer_;
}

template <typename Type, typename Deleter>
void unique_ptr<Type, Deleter>::reset(Type*& pointer) noexcept {
  deleter_(raw_pointer_);
  raw_pointer_ = pointer;
  pointer = nullptr;
}

template <typename Type, typename Deleter>
void unique_ptr<Type, Deleter>::reset(Type*&& pointer) noexcept {
  reset(pointer);
}
} // namespace brocolio::memory