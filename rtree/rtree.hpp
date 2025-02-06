#ifndef RTREE_HPP
#define RTREE_HPP

#define M 7
#define UTILIZATION_FACTOR 0.40f

#include "rect.hpp"

#include <assert.h>
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
  int id_in_parent;

  union {
    Node* children[M + 1];
    Item data[M + 1];
  };

  static Node* empty(Kind kind = LEAF, Node* parent = nullptr) {
    Node* n = new Node;
    n->count = 0;
    n->kind = kind;
    n->parent = parent;

    return n;
  }

  bool is_leaf() { return kind == LEAF; }
  bool is_branch() { return !is_leaf(); }
  bool is_root() { return parent == nullptr; }
  bool is_overflowing() { return count > M; }
  void set_child(int i, Node* child) {
    assert(kind == BRANCH);
    assert(i < M + 1);
    assert(child != nullptr);

    children[i] = child;
    child->id_in_parent = i;
    child->parent = this;
  }
  void recalculate_mbr() {
    if (count == 0)
      return;
    mbr = kind == BRANCH ? children[0]->mbr : data[0].as_rect();
    for (int i = 0; i < count; i++) {
      mbr = merge(mbr, kind == BRANCH ? children[i]->mbr : data[i].as_rect());
    }
  }

  void recalculate_mbr_sift_up () {
    recalculate_mbr();
    if(parent != nullptr) {
      parent->recalculate_mbr_sift_up();
    }
  }
};

class RTree {
public:
  Node* root;

  RTree();
  ~RTree();

  std::vector<Item> search(Rect window);
  void insert(Item item);

private:
  int best_child(Node* node, Item item);
  void handle_overflow(Node* node);
  void search_rec(Node* node, Rect window, std::vector<Item>& results);
  void insert_rec(Node* node, Item item);
};

#endif // RTREE_HPP
