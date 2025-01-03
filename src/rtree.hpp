#ifndef RTREE_HPP
#define RTREE_HPP

#define ITEM_COUNT 125
#define M 6
#define UTILIZATION_FACTOR 0.0f

#include "rect.hpp"
#include <vector>

typedef enum Kind {
  LEAF = 1,
  BRANCH = 2,
} Kind;

typedef struct Item {
  float x;
  float y;
  int id;

  Rect as_rect() { return Rect{x, y, x, y}; }
} Item;

class Node {
public:
  Kind kind;
  int count;
  Rect mbr;
  Node* parent = nullptr;
  int id_in_parent; // Indique où se trouve ce noud dans la liste 'children' de
                    // son parent

  union {
    Node* children[M + 1]; // Si c'est une branche
    Item data[M + 1];      // Si c'est une feuille
  };

  static Node* empty(Kind kind = LEAF, Node* parent = nullptr) {
    Node* n = new Node;
    n->count = 0;
    n->kind = kind;
    n->parent = parent;

    if (kind == BRANCH) {
      for (int i = 0; i < M + 1; i++) {
        n->children[i] = nullptr; // Initialiser les pointeurs enfants
      }
    }

    return n;
  }

  bool is_leaf() { return kind == LEAF; }
  bool is_branch() { return !is_leaf(); }
  bool is_root() { return parent == nullptr; }
  bool is_overflowing() { return count > M; }
  void recalculate_mbr() {
    if (count == 0)
      return;
    mbr = kind == BRANCH ? children[0]->mbr : data[0].as_rect();
    for (int i = 0; i < count; i++) {
      mbr = merge(mbr, kind == BRANCH ? children[i]->mbr : data[i].as_rect());
    }
  }
};

class RTree {
public:
  Node* root;

  RTree();

  std::vector<Item> search(Rect window);
  void insert(Item item);

private:
  void handle_overflow(Node* node);
  void search_rec(Node* node, Rect window, std::vector<Item>& results);
  void insert_rec(Node* node, Item item);
};

#endif // RTREE_HPP
