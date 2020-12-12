#pragma once
#include "stack.hpp"
#include "typenames.hpp"
#include <functional>
#include <iostream>
#include <stdexcept>
namespace brocolio::container {
// binary search tree by Brocolio de la CHUNSA
template <class KeyType>
class binary_search_tree {
public:
  class iterator;
  binary_search_tree() = default;
  binary_search_tree(binary_search_tree const&) noexcept; // TODO
  binary_search_tree(binary_search_tree&&) noexcept;      // TODO
  binary_search_tree(KeyType root_key) noexcept;
  binary_search_tree(
      KeyType root_key,
      std::function<bool(KeyType const&, KeyType const&)> ordering_function);

  binary_search_tree(
      std::function<bool(KeyType const&, KeyType const&)> ordering_function);

  ~binary_search_tree();

  iterator begin() const noexcept { return iterator{this, root_}; }
  iterator end() const noexcept { return iterator{this, nullptr}; }
  bool insert(KeyType key) noexcept;
  bool remove(KeyType key) noexcept;
  bool search(KeyType key) const noexcept;
  std::size_t size() const noexcept { return size_; };
  bool empty() const noexcept { return size_ == 0; };

  void print(transversal_method method = transversal_method::in_order) const;

  KeyType max_key() const;
  KeyType min_key() const;

  KeyType
  successor(KeyType key,
            transversal_method method = transversal_method::in_order) const;

  KeyType
  predecessor(KeyType key,
              transversal_method method = transversal_method::in_order) const;

private:
  struct node;
  node* root_{nullptr};
  bool remove_flag{true};
  std::function<bool(KeyType const&, KeyType const&)> ordering_function_{
      [](KeyType const a, KeyType const b) { return a < b; }};

  std::size_t size_{0};

  node const* search_node(node const* root, KeyType key) const noexcept;
  node* search_node(node* root, KeyType key) noexcept;

  node const* successor_node(node const* given_node,
                             transversal_method method) const noexcept;

  node* successor_node(node* given_node, transversal_method method) noexcept;

  node const* predecessor_node(node const* given_node,
                               transversal_method method) const noexcept;

  node* predecessor_node(node* given_node, transversal_method method) noexcept;

  node const* min_key_node(node const* root) const noexcept;
  node const* max_key_node(node const* root) const noexcept;
  bool remove_node(node* given_node) noexcept;
  void destructor_helper(node* root) noexcept;
};

template <class KeyType>
struct binary_search_tree<KeyType>::node {
  KeyType key{};
  node* left{nullptr};
  node* right{nullptr};
  node* parent{nullptr};
};

template <class KeyType>
class binary_search_tree<KeyType>::iterator {
public:
  iterator() = default;
  iterator(iterator const&) = default;
  iterator(iterator&&) = default;
  iterator(binary_search_tree const* tree, node const* it_node)
      : tree_{tree}, it_node_{it_node} {}
  ~iterator() = default;

  iterator& operator++() {
    it_node_ = tree_->successor_node(it_node_, transversal_method::in_order);
    return *this;
  }

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
binary_search_tree<KeyType>::binary_search_tree(KeyType const root_key) noexcept
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
binary_search_tree<KeyType>::~binary_search_tree() {
  if (root_ != nullptr) destructor_helper(root_);
}

template <class KeyType>
bool binary_search_tree<KeyType>::insert(KeyType const key) noexcept {
  if (root_ == nullptr) {
    root_ = new node{key, nullptr, nullptr, nullptr};
    ++size_;
    return true;
  }

  for (auto current_node{root_};;) {
    if (ordering_function_(key, current_node->key)) {
      if (current_node->left == nullptr) {
        current_node->left = new node{key, nullptr, nullptr, current_node};
        ++size_;
        return true;
      } else {
        current_node = current_node->left;
      }
    } else if (ordering_function_(current_node->key, key)) {
      if (current_node->right == nullptr) {
        current_node->right = new node{key, nullptr, nullptr, current_node};
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
typename binary_search_tree<KeyType>::node const*
binary_search_tree<KeyType>::search_node(node const* const root,
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
typename binary_search_tree<KeyType>::node*
binary_search_tree<KeyType>::search_node(node* const root,
                                         KeyType const key) noexcept {
  return const_cast<node*>(std::as_const(*this).search_node(root, key));
}

template <class KeyType>
void binary_search_tree<KeyType>::print(transversal_method const method) const {
  if (root_ == nullptr) {
    std::cout << "null" << std::endl;
    return;
  }

  stack<node const*> node_stack{};

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

  case transversal_method::in_order:
    for (node const* current_node{root_};
         current_node != nullptr || !node_stack.empty();) {
      while (current_node != nullptr) {
        node_stack.push(current_node);
        current_node = current_node->left;
      }
      current_node = node_stack.pop();
      std::cout << current_node->key << " ";
      current_node = current_node->right;
    }
    break;

  case transversal_method::post_order: {
    node const* previous_node{nullptr};
    node const* current_node{root_};
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
typename binary_search_tree<KeyType>::node const*
binary_search_tree<KeyType>::successor_node(
    node const* const given_node,
    transversal_method const method) const noexcept {
  switch (method) {
  case transversal_method::pre_order:
    if (given_node->left != nullptr) {
      return given_node->left;
    } else if (given_node->right != nullptr) {
      return given_node->right;
    } else {
      auto current_node{given_node};
      auto ancestor{given_node->parent};
      while (ancestor != nullptr) {
        if (ancestor->right != nullptr && ancestor->right != current_node) {
          return ancestor->right;
        } else {
          current_node = current_node->parent;
          ancestor = ancestor->parent;
        }
      }
      return nullptr;
    }
    break;

  case transversal_method::in_order:
    if (given_node->right != nullptr) {
      return min_key_node(given_node->right);
    } else {
      auto parent{given_node->parent};
      auto current_node{given_node};
      while (parent != nullptr && current_node == parent->right) {
        current_node = parent;
        parent = parent->parent;
      }
      return parent;
    }
    break;

  case transversal_method::post_order:
    if (given_node == root_) {
      return nullptr;
    } else if (auto parent{given_node->parent}; given_node == parent->right) {
      return parent;
    } else if (parent->right != nullptr) {
      auto current_node{min_key_node(parent->right)};

      while (current_node->right != nullptr) {
        current_node = min_key_node(current_node->right);
      }
      return current_node;
    } else {
      return parent;
    }
  default: return nullptr;
  }
}

template <class KeyType>
typename binary_search_tree<KeyType>::node*
binary_search_tree<KeyType>::successor_node(
    node* const given_node, transversal_method const method) noexcept {
  return const_cast<node*>(
      std::as_const(*this).successor_node(given_node, method));
}

template <class KeyType>
typename binary_search_tree<KeyType>::node const*
binary_search_tree<KeyType>::predecessor_node(
    node const* const given_node,
    transversal_method const method) const noexcept {
  switch (method) {
  case transversal_method::pre_order:
    if (given_node == root_) {
      return nullptr;
    } else if (auto parent{given_node->parent};
               given_node == parent->left || parent->left == nullptr) {
      return parent;
    } else {
      auto sibling{parent->left};
      while (sibling->right != nullptr) sibling = sibling->right;
      return sibling;
    }
  case transversal_method::in_order:
    if (given_node->left != nullptr) {
      return max_key_node(given_node->left);
    } else {
      node const* ancestor{root_};
      node const* current_node{nullptr};
      while (ancestor->key != given_node->key) {
        if (ordering_function_(ancestor->key, given_node->key)) {
          current_node = ancestor;
          ancestor = ancestor->right;
        } else {
          ancestor = ancestor->left;
        }
      }
      return current_node;
    }
  case transversal_method::post_order:
    if (given_node->right != nullptr) {
      return given_node->right;
    } else if (given_node->left != nullptr) {
      return given_node->left;
    } else {
      auto current_symbol{given_node};
      auto ancestor{given_node->parent};
      while (ancestor != nullptr) {
        if (ancestor->left != nullptr && ancestor->left != current_symbol) {
          return ancestor->left;
        } else {
          current_symbol = current_symbol->parent;
          ancestor = ancestor->parent;
        }
      }
      return nullptr;
    }
  default: return nullptr;
  }
}

template <class KeyType>
typename binary_search_tree<KeyType>::node*
binary_search_tree<KeyType>::predecessor_node(
    node* const given_node, transversal_method const method) noexcept {
  return const_cast<node*>(
      std::as_const(*this).predecessor_node(given_node, method));
}

template <class KeyType>
typename binary_search_tree<KeyType>::node const*
binary_search_tree<KeyType>::min_key_node(
    node const* const root) const noexcept {
  if (root != nullptr) {
    auto current_node{root};
    while (current_node->left != nullptr) current_node = current_node->left;
    return current_node;
  } else {
    return nullptr;
  }
}

template <class KeyType>
typename binary_search_tree<KeyType>::node const*
binary_search_tree<KeyType>::max_key_node(
    node const* const root) const noexcept {
  if (root != nullptr) {
    auto current_node{root};
    while (current_node->right != nullptr) current_node = current_node->right;
    return current_node;
  } else {
    return nullptr;
  }
}

template <class KeyType> // add throw
KeyType binary_search_tree<KeyType>::min_key() const {
  return min_key_node(root_)->key;
}

template <class KeyType> // add throw
KeyType binary_search_tree<KeyType>::max_key() const {
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

template <class KeyType>
bool binary_search_tree<KeyType>::remove_node(node* const given_node) noexcept {
  if (given_node != nullptr) {
    if (given_node->right == nullptr && given_node->left == nullptr) {
      delete given_node;
    } else if (given_node->right != nullptr ^ given_node->left != nullptr) {
      node* child{given_node->left != nullptr ? given_node->left
                                              : given_node->right};
      given_node->key = child->key;
      delete child;
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

template <class KeyType>
void binary_search_tree<KeyType>::destructor_helper(node* root) noexcept {
  if (root->left == nullptr && root->right == nullptr) {
    delete root;
  } else if (root->left != nullptr) {
    destructor_helper(root->left);
  } else if (root->right != nullptr) {
    destructor_helper(root->right);
  }
}
} // namespace brocolio::container
