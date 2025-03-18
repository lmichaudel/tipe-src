#include "benchmarks.hpp"

#include <chrono>

long long benchmark_build(RTree& tree, Item* items, int count, int w, int h) {
  const auto t1 = Clock::now();

  tree.root->mbr = Rect(items[0].as_rect());
  for (int i = 0; i < count; i++) {
    Item item = items[i];
    if (item.x < 0 || item.x > w || item.y < 0 || item.y > h) {
      continue;
    }
    tree.insert(item);
  }

  return std::chrono::duration_cast<std::chrono::milliseconds>(Clock::now() -
                                                               t1)
      .count();
}

std::pair<long long, long long> benchmark_n(RTree& tree, Rect window,
                                            Item* items, int count) {
  // NAIVE
  auto t1 = Clock::now();
  std::vector<Item> query;

  for (int i = 0; i < count; i++) {
    if (intersect(items[i].as_rect(), window)) {
      query.push_back(items[i]);
    }
  }

  auto naive =
      std::chrono::duration_cast<std::chrono::microseconds>(Clock::now() - t1)
          .count();

  // R-Tree
  t1 = Clock::now();
  query = tree.search(window);

  auto rtree =
      std::chrono::duration_cast<std::chrono::microseconds>(Clock::now() - t1)
          .count();

  return {naive, rtree};
}

float get_dst(Item a, Item b) {
  return sqrt((b.x - a.x) * (b.x - a.x) + (b.y - a.y) * (b.y - a.y));
}

std::pair<long long, long long> benchmark_n2(RTree& tree, float dst,
                                             Item* items, int count) {
  float half_dst = dst / 2.0f;
  // NAIVE
  auto t1 = Clock::now();
  std::vector<std::pair<Item, Item>> query;

  for (int i = 0; i < count; i++) {
    for (int j = 0; j < count; j++) {
      if (intersect({items[i].x - half_dst, items[i].y - half_dst,
                     items[i].x + half_dst, items[i].y + half_dst},
                    items[j].as_rect()))
        query.push_back({items[i], items[j]});
    }
  }

  auto naive =
      std::chrono::duration_cast<std::chrono::microseconds>(Clock::now() - t1)
          .count();

  query.clear();

  // R-Tree
  t1 = Clock::now();

  for (int i = 0; i < count; i++) {
    auto q = tree.search({items[i].x - half_dst, items[i].y - half_dst,
                          items[i].x + half_dst, items[i].y + half_dst});
    for (auto entry : q) {
      query.push_back({items[i], entry});
    }
  }

  auto rtree =
      std::chrono::duration_cast<std::chrono::microseconds>(Clock::now() - t1)
          .count();

  return {naive, rtree};
}