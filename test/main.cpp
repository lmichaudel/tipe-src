#include "main.hpp"

#include <iostream>

#define ITEM_COUNT 100000

RTree tree;

Item ITEMS[ITEM_COUNT];
Rect window = {300, 160, 520, 380};

void init() {
  srand(136399);
  for (int i = 0; i < ITEM_COUNT; i++) {
    ITEMS[i].id = i + 1;
    ITEMS[i].x = rand() % (WIDTH - 50) + 30;
    ITEMS[i].y = rand() % (HEIGHT - 30) + 10;
  }
}

void draw_tree_rec(App* s, Node* node, int depth = 0) {
  if (node->kind == LEAF) {
    draw_rect(s, node->mbr.a_x, node->mbr.a_y, node->mbr.b_y - node->mbr.a_y,
              node->mbr.b_x - node->mbr.a_x, 0, 0, 0, 255);
  }

  if (node->kind == BRANCH) {
    for (int i = 0; i < node->count; i++) {
       draw_tree_rec(s, node->children[i], depth + 1);
    }
  }
}
void draw(App* s) {
  draw_tree_rec(s, tree.root);

  for (int i = 0; i < ITEM_COUNT; i++) {
    draw_circle(s, ITEMS[i].x, ITEMS[i].y, .5f);
  }

  draw_rect(s, window.a_x, window.a_y, window.b_y - window.a_y, window.b_x - window.a_x, 255, 255, 255, 255);

  std::vector<Item> query = tree.search(window);
  for (auto entry : query) {
    draw_circle(s, entry.x, entry.y, .5f, 0, 255, 0, 255);
  }
}

int main() {
  App* s = create_application();
  init();

  tree.root->mbr = ITEMS[0].as_rect();
  for (int i = 0; i < ITEM_COUNT; i++) {
    tree.insert(ITEMS[i]);
  }

  run(s, draw);

  return 0;
}
