#pragma once
#include "default_delete.hpp"
#include <cstddef>
#include <iostream>
namespace brocolio::memory {
// smart shared pointer class by Brocolio de la CHUNSA
template <typename Type, typename Deleter = default_delete<Type>>
class shared_ptr {
public:
  constexpr shared_ptr() noexcept {};
  constexpr shared_ptr(std::nullptr_t) noexcept {};
  explicit shared_ptr(Type* pointer) noexcept;
  shared_ptr(shared_ptr const&) noexcept;
  shared_ptr(shared_ptr&&) noexcept;
  ~shared_ptr();
  shared_ptr& operator=(shared_ptr const&) noexcept;
  shared_ptr& operator=(shared_ptr&&) noexcept;
  Type& operator*() const noexcept;
  Type* operator->() const noexcept;
  bool operator==(shared_ptr const&) const noexcept;
  bool operator!=(shared_ptr const&) const noexcept;
  bool operator==(std::nullptr_t) const noexcept;
  bool operator!=(std::nullptr_t) const noexcept;
  std::size_t use_count() const noexcept;
  void reset() noexcept;
  void reset(Type* pointer) noexcept;
  Type* get() const noexcept;

private:
  Type* raw_pointer_{nullptr};
  Deleter deleter_{};
  std::size_t* reference_counter_{nullptr};

  void destructor_helper() const noexcept;
};

template <typename Type, typename Deleter>
shared_ptr<Type, Deleter>::shared_ptr(Type* const pointer) noexcept
: raw_pointer_{pointer}, reference_counter_{pointer == nullptr
                                                ? nullptr
                                                : new std::size_t{1}} {}

template <typename Type, typename Deleter>
shared_ptr<Type, Deleter>::shared_ptr(shared_ptr const& other) noexcept
: raw_pointer_{other.raw_pointer_}, reference_counter_{
                                        other.reference_counter_} {
  if (other.raw_pointer_ != nullptr && other.reference_counter_ != nullptr) {
    ++(*reference_counter_);
  }
}

template <typename Type, typename Deleter>
shared_ptr<Type, Deleter>::shared_ptr(shared_ptr&& other) noexcept
: raw_pointer_{other.raw_pointer_}, reference_counter_{
                                        other.reference_counter_} {
  other.raw_pointer_ = nullptr;
  other.reference_counter_ = nullptr;
}

template <typename Type, typename Deleter>
shared_ptr<Type, Deleter>::~shared_ptr() {
  destructor_helper();
}

template <typename Type, typename Deleter>
shared_ptr<Type, Deleter>&
shared_ptr<Type, Deleter>::operator=(shared_ptr const& other) noexcept {
  destructor_helper();
  if (other.raw_pointer_ != nullptr && other.reference_counter_ != nullptr) {
    ++(*other.reference_counter_);
    raw_pointer_ = other.raw_pointer_;
    reference_counter_ = other.reference_counter_;
  } else {
    raw_pointer_ = nullptr;
    reference_counter_ = nullptr;
  }

  return *this;
}

template <typename Type, typename Deleter>
shared_ptr<Type, Deleter>&
shared_ptr<Type, Deleter>::operator=(shared_ptr&& other) noexcept {
  destructor_helper();
  raw_pointer_ = other.raw_pointer_;
  reference_counter_ = other.reference_counter_;
  other.raw_pointer_ = nullptr;
  other.reference_counter_ = nullptr;

  return *this;
}

template <typename Type, typename Deleter>
Type& shared_ptr<Type, Deleter>::operator*() const noexcept {
  return *raw_pointer_;
}

template <typename Type, typename Deleter>
Type* shared_ptr<Type, Deleter>::operator->() const noexcept {
  return raw_pointer_;
}

template <typename Type, typename Deleter>
bool shared_ptr<Type, Deleter>::operator==(
    shared_ptr const& rhs) const noexcept {
  return raw_pointer_ == rhs.raw_pointer_ &&
         reference_counter_ == rhs.reference_counter_;
}

template <typename Type, typename Deleter>
bool shared_ptr<Type, Deleter>::operator!=(
    shared_ptr const& rhs) const noexcept {
  return !(*this == rhs);
}

template <typename Type, typename Deleter>
bool shared_ptr<Type, Deleter>::operator==(std::nullptr_t) const noexcept {
  return raw_pointer_ == nullptr;
}

template <typename Type, typename Deleter>
bool shared_ptr<Type, Deleter>::operator!=(std::nullptr_t) const noexcept {
  return raw_pointer_ != nullptr;
}

template <typename Type, typename Deleter>
std::size_t shared_ptr<Type, Deleter>::use_count() const noexcept {
  return reference_counter_ != nullptr ? *reference_counter_ : 0;
}

template <typename Type, typename Deleter>
void shared_ptr<Type, Deleter>::reset() noexcept {
  destructor_helper();
  raw_pointer_ = nullptr;
  reference_counter_ = nullptr;
}

template <typename Type, typename Deleter>
void shared_ptr<Type, Deleter>::reset(Type* const pointer) noexcept {
  destructor_helper();
  if (pointer != nullptr) {
    raw_pointer_ = pointer;
    reference_counter_ = new std::size_t{1};
  } else {
    raw_pointer_ = nullptr;
    reference_counter_ = nullptr;
  }
}

template <typename Type, typename Deleter>
Type* shared_ptr<Type, Deleter>::get() const noexcept {
  return raw_pointer_;
}

template <typename Type, typename Deleter>
void shared_ptr<Type, Deleter>::destructor_helper() const noexcept {
  if (reference_counter_ != nullptr) {
    if (*reference_counter_ > 0) --*reference_counter_;

    if (*reference_counter_ == 0) {
      deleter_(raw_pointer_);
      delete reference_counter_;
    }
  }
}
} // namespace brocolio::memory