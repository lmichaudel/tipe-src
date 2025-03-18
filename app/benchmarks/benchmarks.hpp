#ifndef BENCHMARKS_HPP
#define BENCHMARKS_HPP

#include "rtree.hpp"

#include <chrono>

typedef std::chrono::high_resolution_clock Clock;

long long benchmark_build(RTree& tree, Item* items, int count, int w, int h);
std::pair<long long, long long> benchmark_n(RTree& tree, Rect window, Item* items, int count);
std::pair<long long, long long> benchmark_n2(RTree& tree, float dst, Item* items, int count);

#endif //BENCHMARKS_HPP
