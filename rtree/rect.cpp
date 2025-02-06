#include "rect.hpp"
#include <algorithm>

// Constructeur
Rect::Rect(float ax, float ay, float bx, float by)
    : a_x(ax), a_y(ay), b_x(bx), b_y(by) {}

// Calculate the area of the rectangle
float Rect::area() const { return (b_x - a_x) * (b_y - a_y); }

// Calculate the perimeter of the rectangle
float Rect::perimeter() const { return 2.0f * ((b_x - a_x) + (b_y - a_y)); }

float Rect::augmentation(const Rect& r) {
  Rect merged = merge(*this, r);
  return merged.area() - area();
}
// Check if two rectangles intersect
bool intersect(const Rect& r1, const Rect& r2) {
  return !(r2.b_x < r1.a_x || r2.a_x > r1.b_x || r2.b_y < r1.a_y ||
           r2.a_y > r1.b_y);
}

// Merge two rectangles and return the resulting rectangle
Rect merge(const Rect& r1, const Rect& r2) {
  float new_a_x = std::min(r1.a_x, r2.a_x);
  float new_a_y = std::min(r1.a_y, r2.a_y);
  float new_b_x = std::max(r1.b_x, r2.b_x);
  float new_b_y = std::max(r1.b_y, r2.b_y);

  return Rect(new_a_x, new_a_y, new_b_x, new_b_y);
}

Rect merge(const Rect& r1, float x, float y) {
  auto r2 = Rect{x, y, x, y};
  return merge(r1, r2);
}

float enlargement(const Rect& mbr, const Rect& new_rect) {
  Rect merged = merge(mbr, new_rect);
  return merged.area() - mbr.area();
}

Rect inter(const Rect& r1, const Rect& r2) {
  Rect inter;
  inter.a_x = std::max(r1.a_x, r2.a_x);
  inter.a_y = std::max(r1.a_y, r2.a_y);
  inter.b_x = std::min(r1.b_x, r2.b_x);
  inter.b_y = std::min(r1.b_y, r2.b_y);

  if (inter.a_x >= inter.b_x || inter.a_y >= inter.b_y) {
    inter = {0, 0, 0, 0};
  }

  return inter;
}

float dead_space(const Rect& r1, const Rect& r2) {
  return inter(r1, r2).area();
}
