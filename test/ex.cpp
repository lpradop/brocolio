#include <iostream>
struct nodo {
  int nro{0};
  nodo* izq{nullptr};
  nodo* der{nullptr};
  nodo* padre{nullptr};
};

using ArbolBinario = nodo*;

void imprimeNodosInternos(ArbolBinario arbol) {
  // imprime los nodos en preorden
  if (arbol != nullptr) {
    std::cout << arbol->nro << std::endl;
    imprimeNodosInternos(arbol->izq);
    imprimeNodosInternos(arbol->der);
  }
}

int main() {
  ArbolBinario arbol{nullptr};
  // ... codigo donde se insertan nodos ...
  imprimeNodosInternos(arbol);
}