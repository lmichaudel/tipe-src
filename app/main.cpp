#include "main.hpp"

#define SAMPLE_SIZE 500000

RTree tree;
Rect window = {550, 250, 650, 400};

Item items[SAMPLE_SIZE];

void load_database () {
  const std::vector<std::pair<double, double>> data = readBinaryFile("app/input");

  const auto t1 = Clock::now();

  for(int i = 0; i < SAMPLE_SIZE; i++) {
    auto [x, y] = coord_to_pixel(data.data()[i]);
    items[i] = {x, y, i};
  }

  std::cout << "Loaded " << SAMPLE_SIZE << " entries in " <<
    std::chrono::duration_cast<std::chrono::milliseconds>(Clock::now() - t1).count()
      << " milliseconds." << std::endl;
}

void construct_tree () {
  const auto t1 = Clock::now();

  tree.root->mbr = Rect(items[0].as_rect());
  for (const auto item : items) {
    if (item.x < 0 || item.x > WIDTH || item.y < 0 || item.y > HEIGHT) {
      continue;
    }
    tree.insert(item);
  }

  std::cout << "Constructed the R-Tree in "
    << std::chrono::duration_cast<std::chrono::milliseconds>(Clock::now() - t1).count()
      << " milliseconds." << std::endl;
}

std::vector<Item> query_rtree() {
  const auto t1 = Clock::now();
  std::vector<Item> query = tree.search(window);

  std::cout << "Queried (R-Tree) " << query.size() << " items in "
  << std::chrono::duration_cast<std::chrono::milliseconds>(Clock::now() - t1).count()
    << " milliseconds." << std::endl;

  return query;
}

std::vector<Item> query_naive() {
  const auto t1 = Clock::now();
  std::vector<Item> query;

  for (auto item : items) {
    if (intersect(item.as_rect(), window)) {
      query.push_back(item);
    }
  }

  std::cout << "Queried (Naive) " << query.size() << " items in "
  << std::chrono::duration_cast<std::chrono::milliseconds>(Clock::now() - t1).count()
    << " milliseconds." << std::endl;

  return query;
}

void draw_tree_rec(App* s, Node* node, int depth = 0) {
  if (node->kind == LEAF) {
    draw_rect(s, node->mbr.a_x, node->mbr.a_y, node->mbr.b_y - node->mbr.a_y,
              node->mbr.b_x - node->mbr.a_x, 0, 0, 0, 50);
  }
  if (node->kind == BRANCH) {
    for (int i = 0; i < node->count; i++) {
      draw_tree_rec(s, node->children[i], depth + 1);
    }
  }
}

void draw(App* s) {
  draw_tree_rec(s, tree.root);
  for (const auto [x, y, id] : items) {
    draw_circle(s, x, y, .5f, 255, 0, 0, 100);
  }


  std::vector<Item> query1 = query_naive();
  std::vector<Item> query2 = query_rtree();

  draw_rect(s, window.a_x, window.a_y, window.b_y - window.a_y, window.b_x - window.a_x, 0, 0, 255, 255);
  for (const auto [x, y, id] : query1) {
    draw_circle(s, x, y, .7f, 0, 255, 0, 255);
  }
}

int main() {
  App* s = create_application();

  load_database();
  construct_tree();

  run(s, draw);
  return 0;
}
