#include "split_algorithms.hpp"
#include "growable_rect.hpp"
#include "rtree.hpp"
#include <iostream>

#define PARTITION_COUNT (1 << (M + 1))
int get_best_mask_exponential(Rect rects[M + 1]) {
  int current_best = 0;
  double current_best_value = std::numeric_limits<double>::max();

  for (int mask = 0; mask < PARTITION_COUNT; mask++) {
    GrowableRect a, b;
    grow_to_mask(rects, mask, &a, &b);
    if (a.count > M || b.count > M || a.count < UTILIZATION_FACTOR * M ||
        b.count < UTILIZATION_FACTOR * M) {
      // The split is invalid
      continue;
    }

    double ds = a.r.area() + b.r.area();
    if (ds < current_best_value) {
      current_best = mask;
      current_best_value = ds;
    }
  }

  return current_best;
}

int get_best_mask_quadratic(Rect rects[M + 1]) {
  GrowableRect a, b;
  int mask = 0;
  int bj = 0, bi = 0;

  double worst_dead_space = 0;
  for (int i = 0; i < M + 1; i++) {
    for (int j = 0; j < M + 1; j++) {
      if (i == j)
        continue;
      double ds = dead_space(rects[i], rects[j]);
      if (ds > worst_dead_space) {
        bj = j;
        bi = i;
        worst_dead_space = ds;
      }
    }
  }

  insert(rects[bj], &a);
  insert(rects[bi], &b);
  mask = flag(mask, bj);

  std::vector<Rect> remaining(0);

  for (int i = 0; i < remaining.size(); i++) {
    if (i != bj && i != bi) {
      remaining.push_back(rects[i]);
    }
  }

  while (remaining.size() != 0) {
    if (a.count + remaining.size() < m) {
      for (int i = 0; i < remaining.size(); i++) {
        mask = flag(mask, i);
        insert(remaining[i], &a);
      }
      break;
    }
    if (b.count + remaining.size() < m) {
      for (int i = 0; i < remaining.size(); i++) {
        insert(remaining[i], &b);
      }
      break;
    }

    int j = -1;
    int side = 0; // 1 = A, 0 = B
    int p = -1;

    for (int i = 0; i < remaining.size(); i++) {
      double da = dead_space(a.r, remaining[i]);
      double db = dead_space(b.r, remaining[i]);

      if (abs(da - db) > p) {
        p = abs(da - db);
        j = i;
        side = (da - db) >= 0 ? 0 : 1;
      }
    }

    if (side == 1) {
      mask = flag(mask, j);
      insert(remaining[j], &a);
    } else {
      insert(remaining[j], &b);
    }
    remaining.erase(remaining.begin() + j);
  }

  return mask;
}

int get_best_mask_linear(Rect rects[M + 1]) {
  std::cerr << "Not yet implemented!" << std::endl;
  return 0;
}

std::pair<Node*, Node*> split(Node* node, SplitHeuristic heuristic) {
  assert(node->count == M + 1);

  Rect rects[M + 1];
  for (int i = 0; i < M + 1; i++) {
    rects[i] =
        node->kind == BRANCH ? node->children[i]->mbr : node->data[i].as_rect();
  }

  int mask;
  switch (heuristic) {
  case LINEAR:
    mask = get_best_mask_linear(rects);
    break;
  case QUADRATIC:
    mask = get_best_mask_quadratic(rects);
    break;
  case EXPONENTIAL:
    mask = get_best_mask_exponential(rects);
    break;
  }

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