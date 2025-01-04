#include "rtree.hpp"
#include "split_algorithms/split_algorithms.hpp"

int RTree::best_child(Node* node, Item item) {
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

  if (parent == nullptr) {
    root = Node::empty(BRANCH, nullptr);
    root->count = 2;
    root->mbr = merge(u->mbr, v->mbr);
    root->set_child(0, u);
    root->set_child(1, v);
  } else {
    parent->set_child(id_in_parent, u);
    parent->set_child(parent->count, v);
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
    insert_rec(node->children[best_child(node, item)], item);
  }
}

void RTree::search_rec(Node* node, Rect window, std::vector<Item>& results) {
  if (intersect(node->mbr, window)) {
    if (node->is_leaf()) {
      for (int i = 0; i < node->count; i++) {
        Item item = node->data[i];
        if (intersect(item.as_rect(), window))
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

void delete_rec(Node* node) {
  if (node->kind == BRANCH) {
    for (int i = 0; i < node->count; i++)
      delete_rec(node->children[i]);
  }

  delete node;
}
RTree::~RTree() { delete_rec(root); }