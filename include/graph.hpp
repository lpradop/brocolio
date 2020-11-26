#pragma once
#include "algorithm.hpp"
#include "binary_search_tree.hpp"
#include "dynamic_matrix.hpp"
#include "linked_list.hpp"
#include "unordered_pair.hpp"
#include <fstream>
#include <iostream>

namespace brocolio::container {
// graph class by Brocolio de CHUNSA (undirected and simple for now)
template <class DataType> class graph {
public:
  graph() = default;
  graph(graph const&); // TODO
  graph(graph&&);      // TODO
  ~graph() = default;
  bool insert_vertex(DataType const vertex);
  bool insert_edge(DataType const v1, DataType const v2);
  bool empty() const { return vertices_.empty(); }
  void generate_dot_file(std::string const file_name) const; // HACK

  linked_list<simple_node<DataType>>
  adyacent_vertices(DataType const vertex) const; // HACK

  dynamic_matrix<DataType> const& adyacency_matrix() const {
    return adyacency_matrix_;
  };

  dynamic_matrix<DataType> adyacency_matrix() { return adyacency_matrix_; };

private:
  binary_search_tree<DataType> vertices_{};
  linked_list<simple_node<unordered_pair<DataType, DataType>>> edges_{};
  std::string const dot_edge_char_{"--"};
  std::string const dot_ext_{".dot"};
  dynamic_matrix<bool> adyacency_matrix_{};
};

template <class DataType>
bool graph<DataType>::insert_vertex(DataType const vertex) {
  return vertices_.insert(vertex);
}

template <class DataType>
bool graph<DataType>::insert_edge(DataType const v1, DataType const v2) {
  if (v1 != v2 && vertices_.search(v1) && vertices_.search(v2)) {
    unordered_pair<DataType, DataType> tmp{v1, v2};
    auto found_edge{algorithm::search(
        edges_,
        [](unordered_pair<DataType, DataType> x,
           unordered_pair<DataType, DataType> y) { return x == y; },
        tmp)};

    if (found_edge == edges_.end()) {
      edges_.insert(tmp);
      return true;
    }
  }

  return false;
}

template <class DataType>
void graph<DataType>::generate_dot_file(std::string const file_name) const {
  std::ofstream file{file_name + dot_ext_};
  file << "graph{" << std::endl;
  for (auto const& edge : edges_) {
    file << edge.x << dot_edge_char_ << edge.y << std::endl;
  }

  for (auto const& vertex : vertices_) {
    if (adyacent_vertices(vertex).empty()) {
      file << vertex << std::endl;
    }
  }
  file << "}" << std::endl;
  file.close();
}

template <class DataType>
linked_list<simple_node<DataType>>
graph<DataType>::adyacent_vertices(DataType const vertex) const {
  linked_list<simple_node<DataType>> found_vertices{};
  auto it{algorithm::search(
      edges_,
      [](unordered_pair<DataType, DataType> x, DataType const vertex) {
        return x.x == vertex || x.y == vertex;
      },
      vertex)};

  while (it != edges_.end()) {
    DataType const adyacent_vertex{(*it).x != vertex ? (*it).x : (*it).y};
    found_vertices.insert(adyacent_vertex);
    it = algorithm::search(
        edges_, ++it,
        [](unordered_pair<DataType, DataType> x, DataType const vertex) {
          return x.x == vertex || x.y == vertex;
        },
        vertex);
  }
  return std::move(found_vertices);
}

} // namespace brocolio::container
