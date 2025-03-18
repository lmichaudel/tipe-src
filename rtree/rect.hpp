#ifndef RECT_HPP
#define RECT_HPP

class Rect {
public:
  double a_x, a_y, b_x, b_y;

  Rect(double ax, double ay, double bx, double by);
  Rect() : a_x(0), a_y(0), b_x(0), b_y(0) {}
  double area() const;
  double perimeter() const;
  double augmentation(const Rect& r);
  inline double min(int dim) const { return (dim == 0) ? a_x : a_y; }
  inline double max(int dim) const { return (dim == 0) ? b_x : b_y; }
};

bool intersect(const Rect& r1, const Rect& r2);
Rect merge(const Rect& r1, const Rect& r2);
Rect inter(const Rect& r1, const Rect& r2);
double dead_space(const Rect& r1, const Rect& r2);

#endif // RECT_HPP
