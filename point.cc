#include "point.hh"

using namespace std;

Point::Point(double x0, double y0) : x(x0), y(y0) {}
Point::Point() : x(0), y(0) {}

double Point::norm() const {
  return sqrt(normsq());
}

double Point::normsq() const {
  return (x * x + y * y);
}

double Point::arg() const {
  return atan2(y, x);
}

Point& Point::operator=(Point const& rhs) {
  x = rhs.x;
  y = rhs.y;
  return *this;
}

Point& Point::operator+=(Point const& rhs) {
  x += rhs.x;
  y += rhs.y;
  return *this;
}

Point& Point::operator-=(Point const& rhs) {
  x -= rhs.x;
  y -= rhs.y;
  return *this;
}

Point& Point::operator*=(double rhs) {
  x *= rhs;
  y *= rhs;
  return *this;
}

Point& Point::operator/=(double rhs) {
  x /= rhs;
  y /= rhs;
  return *this;
}

const Point Point::operator*(double factor) const {
  return Point(*this) *= factor;
}

const Point Point::operator/(double divisor) const {
  return Point(*this) /= divisor;
}

const Point Point::operator+(Point const& rhs) const {
  return Point(*this) += rhs;
}

const Point Point::operator-(Point const& rhs) const {
  return Point(*this) -= rhs;
}

const Point operator*(double factor, Point const& rhs) {
  return Point(rhs) *= factor;
}

ostream &operator<<(ostream& os, Point const& obj) {
  os << "[" << obj.x << ", " << obj.y << "]";
  return os;
}
