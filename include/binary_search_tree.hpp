#pragma once
#include "memory.hpp"
#include "stack.hpp"
#include "typenames.hpp"
#include <functional>
#include <iostream>
#include <stdexcept>
namespace brocolio::container {
// binary search tree by Brocolio de la CHUNSA
// add noexcept
template <class KeyType> class binary_search_tree {
public:
  class iterator;
  binary_search_tree() = default;
  binary_search_tree(binary_search_tree const&); // TODO
  binary_search_tree(binary_search_tree&&);      // TODO
  binary_search_tree(KeyType const root_key);
  binary_search_tree(
      KeyType const root_key,
      std::function<bool(KeyType const&, KeyType const&)> ordering_function);

  binary_search_tree(
      std::function<bool(KeyType const&, KeyType const&)> ordering_function);

  ~binary_search_tree() = default;

  iterator begin() const { return iterator{this, root_}; }
  iterator end() const { return iterator{this, nullptr}; }
  bool insert(KeyType const key) noexcept;
  bool remove(KeyType const key) noexcept;
  bool search(KeyType const key) const noexcept;
  std::size_t size() const noexcept { return size_; };
  bool empty() const noexcept { return size_ == 0; };

  void
  print(transversal_method const method = transversal_method::in_order) const;

  KeyType max_key() const noexcept;
  KeyType min_key() const noexcept;

  KeyType successor(KeyType const key, transversal_method const method =
                                           transversal_method::in_order) const;

  KeyType predecessor(
      KeyType const key,
      transversal_method const method = transversal_method::in_order) const;

private:
  struct node;
  memory::shared_ptr<node> root_{nullptr};
  bool remove_flag{true};

  std::function<bool(KeyType const&, KeyType const&)> ordering_function_{
      [](KeyType const a, KeyType const b) { return a < b; }};

  std::size_t size_{0};

  memory::shared_ptr<node> search_node(memory::shared_ptr<node> const& root,
                                       KeyType const key) const noexcept;

  memory::shared_ptr<node>
  successor_node(memory::shared_ptr<node> const& given_node,
                 transversal_method const method) const noexcept;

  memory::shared_ptr<node>
  predecessor_node(memory::shared_ptr<node> const& given_node,
                   transversal_method const method) const noexcept;

  memory::shared_ptr<node>
  min_key_node(memory::shared_ptr<node> const& root) const noexcept;

  memory::shared_ptr<node>
  max_key_node(memory::shared_ptr<node> const& root) const noexcept;

  bool remove_node(memory::shared_ptr<node>& given_node);
};

template <class KeyType> struct binary_search_tree<KeyType>::node {
  KeyType key{};
  memory::shared_ptr<node> left{nullptr};
  memory::shared_ptr<node> right{nullptr};
  memory::shared_ptr<node> parent{nullptr};
};

template <class KeyType> class binary_search_tree<KeyType>::iterator {
public:
  iterator() = default;
  iterator(iterator const&) = default;
  iterator(iterator&&) = default;
  iterator(binary_search_tree const* const tree, node const* it_node)
      : tree_(tree), it_node_(it_node) {}
  ~iterator() = default;
  iterator& operator++() {
    it_node_ = tree_->successor_node(it_node_, transversal_method::in_order);
    return *this;
  };

  KeyType const& operator*() const { return it_node_->key; }
  bool operator==(iterator const& other) const {
    return (this->it_node_ == other.it_node_);
  }
  bool operator!=(iterator const& other) const { return !(*this == other); }

private:
  node const* it_node_{nullptr};
  binary_search_tree const* const tree_{nullptr};
};

template <class KeyType>
binary_search_tree<KeyType>::binary_search_tree(KeyType const root_key)
    : root_{new node{root_key, nullptr, nullptr, nullptr}} {}

template <class KeyType>
binary_search_tree<KeyType>::binary_search_tree(
    KeyType const root_key,
    std::function<bool(KeyType const&, KeyType const&)> ordering_function)
    : binary_search_tree{root_key} {
  ordering_function_ = ordering_function;
}

template <class KeyType>
binary_search_tree<KeyType>::binary_search_tree(
    std::function<bool(KeyType const&, KeyType const&)> ordering_function)
    : ordering_function_{ordering_function} {}

template <class KeyType>
bool binary_search_tree<KeyType>::insert(KeyType const key) noexcept {
  if (root_ == nullptr) {
    root_ = memory::shared_ptr{new node{key, nullptr, nullptr, nullptr}};
    ++size_;
    return true;
  } else {
    auto current_node{root_};
    while (true) {
      if (ordering_function_(key, current_node->key)) {
        if (current_node->left == nullptr) {
          current_node->left =
              memory::shared_ptr{new node{key, nullptr, nullptr, current_node}};
          ++size_;
          return true;
        } else {
          current_node = current_node->left;
        }
      } else if (ordering_function_(current_node->key, key)) {
        if (current_node->right == nullptr) {
          current_node->right =
              memory::shared_ptr{new node{key, nullptr, nullptr, current_node}};
          ++size_;
          return true;
        } else {
          current_node = current_node->right;
        }
      } else {
        return false;
      }
    }
  }
}

template <class KeyType>
bool binary_search_tree<KeyType>::remove(KeyType const key) noexcept {
  if (auto found_node{search_node(root_, key)}; found_node == nullptr) {
    return false;
  } else {
    remove_node(found_node);
    --size_;
    return true;
  }
}

template <class KeyType>
bool binary_search_tree<KeyType>::search(KeyType const key) const noexcept {
  if (auto found_node{search_node(root_, key)}; found_node == nullptr) {
    return false;
  } else {
    return true;
  }
}

template <class KeyType>
memory::shared_ptr<typename binary_search_tree<KeyType>::node>
binary_search_tree<KeyType>::search_node(memory::shared_ptr<node> const& root,
                                         KeyType const key) const noexcept {
  for (auto current_node{root}; current_node != nullptr;) {
    if (key == current_node->key) {
      return current_node;
    } else if (ordering_function_(key, current_node->key)) {
      current_node = current_node->left;
    } else {
      current_node = current_node->right;
    }
  }
  return nullptr;
}

template <class KeyType>
void binary_search_tree<KeyType>::print(transversal_method const method) const {
  if (root_ == nullptr) {
    std::cout << "null" << std::endl;
    return;
  }

  stack<memory::shared_ptr<node>> node_stack{};

  switch (method) {
  case transversal_method::pre_order:
    node_stack.push(root_);
    while (!node_stack.empty()) {
      auto current_node{node_stack.top()};

      std::cout << current_node->key << " ";
      node_stack.pop();
      if (current_node->right != nullptr) {
        node_stack.push(current_node->right);
      }
      if (current_node->left != nullptr) {
        node_stack.push(current_node->left);
      }
    }
    break;

  case transversal_method::in_order: {
    auto current_node{root_};
    while (current_node != nullptr || !node_stack.empty()) {
      while (current_node != nullptr) {
        node_stack.push(current_node);
        current_node = current_node->left;
      }
      current_node = node_stack.pop();
      std::cout << current_node->key << " ";
      current_node = current_node->right;
    }
    break;
  }

  case transversal_method::post_order: {
    memory::shared_ptr<node> previous_node{nullptr};
    auto current_node{root_};
    while (current_node != nullptr || !node_stack.empty()) {
      if (current_node != nullptr) {
        node_stack.push(current_node);
        current_node = current_node->left;
      } else {
        current_node = node_stack.top();
        if (current_node->right == nullptr ||
            current_node->right == previous_node) {
          std::cout << current_node->key << " ";
          node_stack.pop();
          previous_node = current_node;
          current_node = nullptr;
        } else {
          current_node = current_node->right;
        }
      }
    }
    break;
  }
  }
  std::cout << std::endl;
};

template <class KeyType>
memory::shared_ptr<typename binary_search_tree<KeyType>::node>
binary_search_tree<KeyType>::successor_node(
    memory::shared_ptr<node> const& given_node,
    transversal_method const method) const noexcept {
  switch (method) {
  case transversal_method::pre_order:
    if (given_node->left != nullptr) {
      return given_node->left;
    } else if (given_node->right != nullptr) {
      return given_node->right;
    } else {
      auto tmp{given_node};
      auto ancestor{given_node->parent};
      while (ancestor != nullptr) {
        if (ancestor->right != nullptr && ancestor->right != tmp) {
          return ancestor->right;
        } else {
          tmp = tmp->parent;
          ancestor = ancestor->parent;
        }
      }
      return nullptr;
    }

  case transversal_method::in_order:
    if (given_node->right != nullptr) {
      return min_key_node(given_node->right);
    } else {
      auto parent{given_node->parent};
      auto tmp{given_node};
      while (parent != nullptr && tmp == parent->right) {
        tmp = parent;
        parent = parent->parent;
      }
      return parent;
    }

  case transversal_method::post_order:
    if (given_node == root_) {
      return nullptr;
    } else if (auto parent{given_node->parent}; given_node == parent->right) {
      return parent;
    } else if (parent->right != nullptr) {
      auto tmp{min_key_node(parent->right)};
      while (tmp->right != nullptr) {
        tmp = min_key_node(tmp->right);
      }
      return tmp;
    } else {
      return parent;
    }
  default:
    return nullptr;
  }
}

template <class KeyType>
memory::shared_ptr<typename binary_search_tree<KeyType>::node>
binary_search_tree<KeyType>::predecessor_node(
    memory::shared_ptr<node> const& given_node,
    transversal_method const method) const noexcept {
  switch (method) {
  case transversal_method::pre_order:
    if (given_node == root_) {
      return nullptr;
    } else if (auto parent{given_node->parent};
               given_node == parent->left || parent->left == nullptr) {
      return parent;
    } else {
      auto tmp{parent->left};
      while (tmp->right != nullptr) {
        tmp = tmp->right;
      }
      return tmp;
    }
  case transversal_method::in_order:
    if (given_node->left != nullptr) {
      return max_key_node(given_node->left);
    } else {
      auto ancestor{root_};
      memory::shared_ptr<node> tmp{nullptr};
      while (ancestor->key != given_node->key) {
        if (ordering_function_(ancestor->key, given_node->key)) {
          tmp = ancestor;
          ancestor = ancestor->right;
        } else {
          ancestor = ancestor->left;
        }
      }
      return tmp;
    }
  case transversal_method::post_order:
    if (given_node->right != nullptr) {
      return given_node->right;
    } else if (given_node->left != nullptr) {
      return given_node->left;
    } else {
      auto tmp{given_node};
      auto ancestor{given_node->parent};
      while (ancestor != nullptr) {
        if (ancestor->left != nullptr && ancestor->left != tmp) {
          return ancestor->left;
        } else {
          tmp = tmp->parent;
          ancestor = ancestor->parent;
        }
      }
      return nullptr;
    }
  default:
    return nullptr;
  }
}

template <class KeyType>
memory::shared_ptr<typename binary_search_tree<KeyType>::node>
binary_search_tree<KeyType>::min_key_node(
    memory::shared_ptr<node> const& root) const noexcept {
  if (root != nullptr) {
    auto current_node{root};
    while (current_node->left != nullptr) {
      current_node = current_node->left;
    }
    return current_node;
  } else {
    return nullptr;
  }
}

template <class KeyType>
memory::shared_ptr<typename binary_search_tree<KeyType>::node>
binary_search_tree<KeyType>::max_key_node(
    memory::shared_ptr<node> const& root) const noexcept {
  if (root != nullptr) {
    auto current_node{root};
    while (current_node->right != nullptr) {
      current_node = current_node->right;
    }
    return current_node;
  } else {
    return nullptr;
  }
}

template <class KeyType>
KeyType binary_search_tree<KeyType>::min_key() const noexcept {
  return min_key_node(root_)->key;
}

template <class KeyType>
KeyType binary_search_tree<KeyType>::max_key() const noexcept {
  return max_key_node(root_)->key;
}

template <class KeyType>
KeyType
binary_search_tree<KeyType>::successor(KeyType const key,
                                       transversal_method const method) const {
  if (auto found_node{search_node(root_, key)}; found_node != nullptr) {
    auto found_node_successor{successor_node(found_node, method)};
    return found_node_successor != nullptr ? found_node_successor->key : key;
  } else {
    throw std::domain_error{"key not found"};
  }
}

template <class KeyType>
KeyType binary_search_tree<KeyType>::predecessor(
    KeyType const key, transversal_method const method) const {
  if (auto found_node{search_node(root_, key)}; found_node != nullptr) {
    auto found_node_predecessor{predecessor_node(found_node, method)};
    return found_node_predecessor != nullptr ? found_node_predecessor->key
                                             : key;
  } else {
    throw std::domain_error{"key not found"};
  }
}

template <class KeyType> // refactor
bool binary_search_tree<KeyType>::remove_node(
    memory::shared_ptr<node>& given_node) {
  if (given_node != nullptr) {
    if (given_node->right == nullptr && given_node->left == nullptr) {
      given_node.reset(nullptr);
    } else if (given_node->right != nullptr ^ given_node->left != nullptr) {
      auto child{given_node->left != nullptr ? given_node->left
                                             : given_node->right};
      given_node->key = child->key;
      child.reset(nullptr);
    } else {
      auto tmp{
          remove_flag
              ? successor_node(given_node, transversal_method::in_order)
              : predecessor_node(given_node, transversal_method::in_order)};
      given_node->key = tmp->key;
      remove_node(tmp);
      remove_flag = !remove_flag;
    }
    return true;
  } else {
    return false;
  }
}

} // namespace brocolio::container
