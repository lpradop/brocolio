#pragma once
#include <functional>
#include <iostream>
#include <stdexcept>

namespace brocolio::container {
// binary search tree by Brocolio de la CHUNSA
enum class transversal_method { pre_order, in_order, post_order };

template <class KeyType> class binary_search_tree {
public:
  binary_search_tree() = default;
  binary_search_tree(binary_search_tree const&); // TODO
  binary_search_tree(binary_search_tree&&);      // TODO
  binary_search_tree(KeyType const root_key);
  binary_search_tree(
      KeyType const root_key,
      std::function<bool(KeyType const, KeyType const)> ordering_function);
  ~binary_search_tree(); // TODO

  bool insert(KeyType const key);
  bool remove(KeyType const key); // TODO
  bool search(KeyType const key) const;

  void print(transversal_method const method =
                 transversal_method::in_order) const; // TODO

  KeyType max_key();
  KeyType min_key();

  KeyType
  successor(KeyType const key,
            transversal_method const method = transversal_method::in_order);

  KeyType
  predecessor(KeyType const key,
              transversal_method const method = transversal_method::in_order);

private:
  struct node;
  node* root_{nullptr};
  std::function<bool(KeyType const, KeyType const)> ordering_function_{
      [](KeyType const a, KeyType const b) { return a < b; }};

  node const* search_node(node const* const root, KeyType const key) const;

  node const* successor_node(node const* const some_node,
                             transversal_method const method) const;

  node const* predecessor_node(node const* const some_node,
                               transversal_method const method) const;

  node const* min_key_node(node const* const root) const;
  node const* max_key_node(node const* const root) const;
};

template <class KeyType> struct binary_search_tree<KeyType>::node {
  KeyType key{};
  node* left{nullptr};
  node* right{nullptr};
  node* parent{nullptr};
};

template <class KeyType>
binary_search_tree<KeyType>::binary_search_tree(KeyType const root_key)
    : root_(new node{root_key, nullptr, nullptr, nullptr}) {}

template <class KeyType>
binary_search_tree<KeyType>::binary_search_tree(
    KeyType const root_key,
    std::function<bool(KeyType const, KeyType const)> ordering_function)
    : binary_search_tree(root_key) {
  ordering_function_ = ordering_function;
}

template <class KeyType> binary_search_tree<KeyType>::~binary_search_tree() {
  // TODO
}

template <class KeyType>
bool binary_search_tree<KeyType>::insert(KeyType const key) {
  if (root_ == nullptr) {
    root_ = new node{key, nullptr, nullptr, nullptr};
    return true;
  } else {
    node* tmp{root_};
    while (true) {
      if (ordering_function_(key, tmp->key)) {
        if (tmp->left == nullptr) {
          tmp->left = new node{key, nullptr, nullptr, tmp};
          return true;
        } else {
          tmp = tmp->left;
        }
      } else if (ordering_function_(tmp->key, key)) {
        if (tmp->right == nullptr) {
          tmp->right = new node{key, nullptr, nullptr, tmp};
          return true;
        } else {
          tmp = tmp->right;
        }
      } else {
        return false;
      }
    }
  }
}

template <class KeyType>
bool binary_search_tree<KeyType>::remove(KeyType const key) {
  if (auto tmp{search_node(root_, key)}; tmp == nullptr) {
    return false;
  } else {
    if (tmp->left == nullptr and tmp->right == nullptr) {
      delete tmp;
    } else if (tmp->left == nullptr xor tmp->right == nullptr) {
      node* child{tmp->left != nullptr ? tmp->left : tmp->right};
      tmp->key = child->key;
      delete child;
    } else {
      // TODO
    }
    return true;
  }
}

template <class KeyType>
bool binary_search_tree<KeyType>::search(KeyType const key) const {
  if (auto tmp{search_node(root_, key)}; tmp == nullptr) {
    return false;
  } else {
    return true;
  }
}

template <class KeyType>
typename binary_search_tree<KeyType>::node const*
binary_search_tree<KeyType>::search_node(node const* const root,
                                         KeyType const key) const {
  for (node const* tmp{root}; tmp != nullptr;) {
    if (key == tmp->key) {
      return tmp;
    } else if (ordering_function_(key, tmp->key)) {
      tmp = tmp->left;
    } else {
      tmp = tmp->right;
    }
  }
  return nullptr;
}

template <class KeyType>
void binary_search_tree<KeyType>::print(transversal_method const method) const {

};

template <class KeyType>
typename binary_search_tree<KeyType>::node const*
binary_search_tree<KeyType>::successor_node(
    node const* const some_node, transversal_method const method) const {
  switch (method) {
  case transversal_method::pre_order:
    if (some_node->left != nullptr) {
      return some_node->left;
    } else if (some_node->right != nullptr) {
      return some_node->right;
    } else {
      node const* tmp{some_node};
      node const* ancestor{some_node->parent};
      while (ancestor != nullptr) {
        if (ancestor->right != nullptr and ancestor->right != tmp) {
          return ancestor->right;
        } else {
          tmp = tmp->parent;
          ancestor = ancestor->parent;
        }
      }
      return nullptr;
    }
  case transversal_method::in_order:
    if (some_node->right != nullptr) {
      return min_key_node(some_node->right);
    } else {
      node const* parent{some_node->parent};
      node const* tmp{some_node};
      while (parent != nullptr and tmp == parent->right) {
        tmp = parent;
        parent = parent->parent;
      }
      return parent;
    }
  case transversal_method::post_order:
    if (some_node == root_) {
      return nullptr;
    } else if (auto parent{some_node->parent}; some_node == parent->right) {
      return parent;
    } else if (parent->right != nullptr) {
      node const* tmp{min_key_node(parent->right)};
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
typename binary_search_tree<KeyType>::node const*
binary_search_tree<KeyType>::predecessor_node(
    node const* const some_node, transversal_method const method) const {
  switch (method) {
  case transversal_method::pre_order:
    if (some_node == root_) {
      return nullptr;
    } else if (auto parent{some_node->parent};
               some_node == parent->left or parent->left == nullptr) {
      return parent;
    } else {
      node const* tmp{parent->left};
      while (tmp->right != nullptr) {
        tmp = tmp->right;
      }
      return tmp;
    }
  case transversal_method::in_order:
    if (some_node->left != nullptr) {
      return max_key_node(some_node->left);
    } else {
      node const* ancestor{root_};
      node const* tmp{nullptr};
      while (ancestor->key != some_node->key) {
        if (ordering_function_(ancestor->key, some_node->key)) {
          tmp = ancestor;
          ancestor = ancestor->right;
        } else {
          ancestor = ancestor->left;
        }
      }
      return tmp;
    }
  case transversal_method::post_order:
    if (some_node->right != nullptr) {
      return some_node->right;
    } else if (some_node->left != nullptr) {
      return some_node->left;
    } else {
      node const* tmp{some_node};
      node const* ancestor{some_node->parent};
      while (ancestor != nullptr) {
        if (ancestor->left != nullptr and ancestor->left != tmp) {
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
typename binary_search_tree<KeyType>::node const*
binary_search_tree<KeyType>::min_key_node(node const* const root) const {
  if (root != nullptr) {
    node const* tmp{root};
    while (tmp->left != nullptr) {
      tmp = tmp->left;
    }
    return tmp;
  } else {
    return nullptr;
  }
}

template <class KeyType>
typename binary_search_tree<KeyType>::node const*
binary_search_tree<KeyType>::max_key_node(node const* const root) const {
  if (root != nullptr) {
    node const* tmp{root};
    while (tmp->right != nullptr) {
      tmp = tmp->right;
    }
    return tmp;
  } else {
    return nullptr;
  }
}

template <class KeyType> KeyType binary_search_tree<KeyType>::min_key() {
  return min_key_node(root_)->key;
}

template <class KeyType> KeyType binary_search_tree<KeyType>::max_key() {
  return max_key_node(root_)->key;
}

template <class KeyType>
KeyType
binary_search_tree<KeyType>::successor(KeyType const key,
                                       transversal_method const method) {
  if (auto tmp{search_node(root_, key)}; tmp != nullptr) {
    auto tmp_successor{successor_node(tmp, method)};
    return tmp_successor != nullptr ? tmp_successor->key : key;
  } else {
    throw std::domain_error{"key not found"};
  }
}

template <class KeyType>
KeyType
binary_search_tree<KeyType>::predecessor(KeyType const key,
                                         transversal_method const method) {
  if (auto tmp{search_node(root_, key)}; tmp != nullptr) {
    auto tmp_predecessor{predecessor_node(tmp, method)};
    return tmp_predecessor != nullptr ? tmp_predecessor->key : key;
  } else {
    throw std::domain_error{"key not found"};
  }
}

} // namespace brocolio::container
