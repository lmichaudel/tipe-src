#ifndef RECT_HPP
#define RECT_HPP

class Rect {
public:
  float a_x, a_y, b_x, b_y;

  Rect(float ax, float ay, float bx, float by);
  Rect() : a_x(0), a_y(0), b_x(0), b_y(0) {}
  float area() const;
  float perimeter() const;
  float augmentation(const Rect& r);
  bool contains(int x, int y);
};

bool intersect(const Rect& r1, const Rect& r2);
Rect merge(const Rect& r1, const Rect& r2);
Rect inter(const Rect& r1, const Rect& r2);
float dead_space(const Rect& r1, const Rect& r2);
#endif // RECT_HPP
