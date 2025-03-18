#include "main.hpp"

#include "rtree.hpp"

#include "benchmarks/benchmarks.hpp"
#include <cstdio>
#include <machine/limits.h>

#define MODE QUADRATIC
#define SAMPLE_SIZE 500000

// The dataset is stored outside of the repository as its 207mo
// us_places : 12'993'248 places accross USA
// 7.95gb raw ; 200mb extracted and stripped to a list of coordinates only (no
// metadata) source:
// https://archive.org/details/2011-08-SimpleGeo-CC0-Public-Spaces

#define INPUT_FILE "/Users/lucas/TIPE/dataset/us_places"
#define OUTPUT_FILE "/Users/lucas/TIPE/tipe/app/benchmarks/out.csv"

RTree tree{MODE};
Rect window = {550, 250, 650, 400};

Item items[SAMPLE_SIZE];

void load_database() {
  const std::vector<std::pair<double, double>> data =
      readBinaryFile(INPUT_FILE);

  for (int i = 0; i < SAMPLE_SIZE; i++) {
    auto [x, y] = coord_to_pixel(data.data()[i]);
    items[i] = {x, y, i};
  }
}

long long construct_tree() {
  const auto t1 = Clock::now();

  tree.root->mbr = Rect(items[0].as_rect());
  for (const auto item : items) {
    if (item.x < 0 || item.x > WIDTH || item.y < 0 || item.y > HEIGHT) {
      continue;
    }
    tree.insert(item);
  }
  auto time =
      std::chrono::duration_cast<std::chrono::milliseconds>(Clock::now() - t1)
          .count();
  printf("Constructed the R-Tree in %lld milliseconds\n", time);

  return time;
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
    draw_circle(s, x, y, .1f, 255, 0, 0, 100);
  }

  std::vector<Item> query = tree.search(window);

  draw_rect(s, window.a_x, window.a_y, window.b_y - window.a_y,
            window.b_x - window.a_x, 0, 0, 255, 255);
  for (const auto [x, y, id] : query) {
    draw_circle(s, x, y, 1.f, 0, 255, 0, 255);
  }
}

void complete_bench(int count, int steps) {
  std::ofstream output_file(OUTPUT_FILE);
  if (!output_file) {
    std::cerr << "Error: Unable to open file." << std::endl;
    return;
  }
  output_file << "Count; Build Time; Naive Search; R-Tree Search\n";

  for (int i = 1; i < count; i++) {
    RTree t{MODE};
    auto build_time = benchmark_build(t, items, i * steps, WIDTH, HEIGHT);

    auto [t1, t2] = benchmark_n(t, window, items, i * steps);
    // auto [t1, t2] = benchmark_n2(tree, 0.1f, items, i * steps);
    output_file << i * steps << ";" << build_time << ";" << t1 << ";" << t2
                << "\n";
  }

  output_file.close();
}

int main() {
  App* s = create_application();

  load_database();

  // complete_bench(20, (int)(SAMPLE_SIZE / 20));

  construct_tree();

  //
  //  benchmark_n(tree, window, items, SAMPLE_SIZE);
  //  benchmark_n2(tree, 10.0f, items,  SAMPLE_SIZE);

  run(s, draw);
  return 0;
}
