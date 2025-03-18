#ifndef SPLIT_ALGORITHMS_HPP
#define SPLIT_ALGORITHMS_HPP

#include "rtree.hpp"

int get_best_mask_exponential(Rect rects[M + 1]);
int get_best_mask_quadratic(Rect rects[M + 1]);
int get_best_mask_linear(Rect rects[M + 1]);

std::pair<Node*, Node*> split(Node* node, SplitHeuristic heuristic);

#endif // SPLIT_ALGORITHMS_HPP
