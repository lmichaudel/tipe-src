#ifndef GROWABLE_RECT_H
#define GROWABLE_RECT_H

#include "rtree.hpp"

struct GrowableRect {
  Rect r;
  bool init;
  int count;

  GrowableRect() : init(false), count(0) {}
};

inline bool flagged(int mask, int i) { return mask & 1 << i; }
inline int flag(int mask, int i) { return mask | (1 << i); }

inline void grow_to_mask(Rect rects[M + 1], int mask, GrowableRect* u,
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

inline void insert(Rect rect, GrowableRect* t) {
  if (!t->init)
    t->r = rect;

  t->r = merge(t->r, rect);
  t->count++;
  t->init = true;
}

#endif // GROWABLE_RECT_H
