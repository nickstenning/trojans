#include "point.hh"

using namespace std;

Point::Point(double x0, double y0) : x(x0), y(y0) {}
Point::Point() : x(0), y(0) {}

double Point::norm() const {
  return sqrt(x * x + y * y);
}

std::string Point::tabulate() const {
	stringstream out;
  out << x << "\t" << y;
	return out.str();
}

Point & Point::operator+=(const Point &rhs) {
  x += rhs.x;
  y += rhs.y;
  return *this;
}

Point & Point::operator-=(const Point &rhs) {
  x -= rhs.x;
  y -= rhs.y;
  return *this;
}

Point & Point::operator*=(double rhs) {
  x *= rhs;
  y *= rhs;
  return *this;
}

Point & Point::operator/=(double rhs) {
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

const Point Point::operator+(const Point &rhs) const {
  return Point(*this) += rhs;
}

const Point Point::operator-(const Point &rhs) const {
  return Point(*this) -= rhs;
}

const Point operator*(double factor, const Point &rhs) {
  return Point(rhs) *= factor;
}

ostream &operator<<(ostream &os, const Point &obj) {
  os << "[" << obj.x << ", " << obj.y << "]";
  return os;
}