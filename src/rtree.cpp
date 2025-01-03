#include "rtree.hpp"
#include "split_algorithms/split_algorithm.hpp"

int best_child(Node* node, Item item) {
  int id = -1;
  double min_augment = std::numeric_limits<double>::max();

  for (int i = 0; i < node->count; i++) {
    float augment = node->children[i]->mbr.augmentation(item.as_rect());
    if (augment < min_augment) {
      min_augment = augment;
      id = i;
    }
  }

  return id;
}

void RTree::handle_overflow(Node* node) {
  Node* parent = node->parent;
  int id_in_parent = node->id_in_parent;

  auto [u, v] = split_exp(node);

  delete node;

  // Recréation de la racine
  if (parent == nullptr) {
    Node* new_root = Node::empty(BRANCH, nullptr);
    new_root->count = 2;
    new_root->mbr = merge(u->mbr, v->mbr);
    new_root->children[0] = u;
    new_root->children[1] = v;

    u->parent = new_root;
    u->id_in_parent = 0;
    v->parent = new_root;
    v->id_in_parent = 1;

    root = new_root;
  } else {
    parent->children[id_in_parent] = u;
    parent->children[parent->count] = v;

    u->id_in_parent = id_in_parent;
    v->id_in_parent = parent->count;
    u->parent = parent;
    v->parent = parent;

    parent->count++;
    parent->recalculate_mbr();

    if (parent->is_overflowing()) {
      handle_overflow(parent);
    }
  }
}

void RTree::insert_rec(Node* node, Item item) {
  if (node->is_leaf()) {
    node->data[node->count] = item;
    node->count++;
    node->mbr = merge(node->mbr, item.as_rect());

    if (node->is_overflowing()) {
      handle_overflow(node);
    }
  } else {
    int best_child_index = best_child(node, item);
    insert_rec(node->children[best_child_index], item);
  }
}

void RTree::search_rec(Node* node, Rect window, std::vector<Item>& results) {
  if (intersect(node->mbr, window)) {
    if (node->is_leaf()) {
      for (int i = 0; i < node->count; i++) {
        Item item = node->data[i];
        if (window.contains(item.x, item.y))
          results.push_back(item);
      }
    } else {
      for (int i = 0; i < node->count; i++) {
        search_rec(node->children[i], window, results);
      }
    }
  }
}

std::vector<Item> RTree::search(Rect window) {
  std::vector<Item> result;
  search_rec(root, window, result);
  return result;
}

void RTree::insert(Item item) { insert_rec(root, item); }

RTree::RTree() {
  auto n = Node::empty(LEAF, nullptr);

  n->count = 0;
  n->mbr = Rect(0, 0, 0, 0);

  root = n;
}