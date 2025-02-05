#include "main.hpp"
#include "loader.hpp"

#define SAMPLE_SIZE 2500
RTree tree;
Item ITEMS[ITEM_COUNT];
std::vector<std::pair<double, double>> data;

SDL_Texture* mapTexture;

std::pair<int, int> coord_to_pixel(const std::pair<double, double>& coord) {
  double xNorm = (coord.first - MIN_LON) / (MAX_LON - MIN_LON);
  double yNorm = 1.0 - (coord.second - MIN_LAT) / (MAX_LAT - MIN_LAT);

  int x = static_cast<int>(xNorm * WIDTH);
  int y = static_cast<int>(yNorm * HEIGHT);

  return {x, y};
}

void init(App* s) {
  mapTexture = load_texture(s, "app/usa.png");
  data = parse_geo_json("app/chainness.geojson", SAMPLE_SIZE);
}

void draw_tree_rec(App* s, Node* node, int depth = 0) {
  if (node->kind == LEAF) {
    draw_rect(s, node->mbr.a_x, node->mbr.a_y, node->mbr.b_y - node->mbr.a_y,
              node->mbr.b_x - node->mbr.a_x, 0, 0, 0);
  }
  if (node->kind == BRANCH) {
    for (int i = 0; i < node->count; i++) {
      draw_tree_rec(s, node->children[i], depth + 1);
    }
  }
}

void draw(App* s) {
  // SDL_Rect mapRect = {0, 0, WIDTH, HEIGHT};
  // SDL_RenderCopy(s->renderer, mapTexture, nullptr, &mapRect);

  for (auto entry : data) {
    auto [x, y] = coord_to_pixel(entry);
    draw_circle(s, x, y, 1.f);
  }

  draw_tree_rec(s, tree.root);
}

int main() {
  App* s = create_application();
  init(s);

  std::cout << "Loaded " << data.size() << " locations." << std::endl;

  auto [x0, y0] = coord_to_pixel(data.data()[0]);
  tree.root->mbr = Rect(x0, y0, x0, y0);
  for (int i = 0; i < data.size(); i++) {
    auto [x, y] = coord_to_pixel(data.data()[i]);
    if (x < 0 || x > WIDTH || y < 0 || y > HEIGHT) { // Hawaii !
      continue;
    }
    tree.insert(Item{static_cast<float>(x), static_cast<float>(y), i});
  }

  run(s, draw);

  return 0;
}
