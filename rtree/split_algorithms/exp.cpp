#include "split_algorithms.hpp"
#include <assert.h>

#define PARTITION_COUNT (1 << (M + 1))

struct GrowableRect {
  Rect r;
  bool init;
  int count;

  GrowableRect() : init(false), count(0) {}
};

bool flagged(int mask, int i) { return mask & 1 << i; }

void grow_to_mask(Rect rects[M + 1], int mask, GrowableRect* u,
                  GrowableRect* v) {
  for (int i = 0; i < M + 1; i++) {
    GrowableRect* t = flagged(mask, i) ? u : v;
    if (!t->init)
      t->r = rects[i];

    t->r = merge(t->r, rects[i]);
    t->count++;
    t->init = true;
  }
}

// Calcutate the best distribution of M+1 rectangles as a bitmask
int best_distribution(Rect rects[M + 1]) {
  int current_best = 0;
  float current_best_value = std::numeric_limits<float>::max();

  for (int m = 0; m < PARTITION_COUNT; m++) {
    GrowableRect a, b;
    grow_to_mask(rects, m, &a, &b);
    if (a.count > M || b.count > M || a.count < UTILIZATION_FACTOR * M ||
        b.count < UTILIZATION_FACTOR * M) {
      // The split is invalid
      continue;
    }

    float ds = dead_space(a.r, b.r);
    if (ds < current_best_value) {
      current_best = m;
      current_best_value = ds;
    }
  }

  return current_best;
}

// Splits an overflowing node (i.e node->count = M+1) into 2 nodes
std::pair<Node*, Node*> split_exp(Node* node) {
  assert(node->count == M + 1);

  Rect rects[M + 1];
  for (int i = 0; i < M + 1; i++) {
    rects[i] =
        node->kind == BRANCH ? node->children[i]->mbr : node->data[i].as_rect();
  }
  int mask = best_distribution(rects);

  Node* u = Node::empty(node->kind, node->parent);
  Node* v = Node::empty(node->kind, node->parent);
  GrowableRect u_r;
  GrowableRect v_r;

  for (int i = 0; i < M + 1; i++) {
    Node* t = flagged(mask, i) ? u : v;

    if (node->kind == BRANCH) {
      t->children[t->count] = node->children[i];
      node->children[i]->parent = t;
      node->children[i]->id_in_parent = t->count;
    } else {
      t->data[t->count] = node->data[i];
    }

    t->count++;
  }

  grow_to_mask(rects, mask, &u_r, &v_r);
  u->mbr = u_r.r;
  v->mbr = v_r.r;

  return std::pair(u, v);
}