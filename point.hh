#ifndef POINT_HH_
#define POINT_HH_

#include <cmath>
#include <sstream>

class Point
{
public:
  Point();
  Point(double xx, double yy);

  double norm() const;
  double normsq() const;
  double arg() const;

  Point& operator=(Point const& rhs);
  Point& operator+=(Point const& rhs);
  Point& operator-=(Point const& rhs);
  Point& operator*=(double rhs);
  Point& operator/=(double rhs);

  const Point operator*(double factor) const;
  const Point operator/(double divisor) const;
  const Point operator+(Point const& rhs) const;
  const Point operator-(Point const& rhs) const;

  friend std::ostream &operator<<(std::ostream& os, Point const& obj);

  double x, y;
};

// Allow "2 * Point(0, 4)" as well. Member function only
// allows "Point(0, 4) * 2".
const Point operator*(double factor, Point const& rhs);

// Syntactic sugar for when we're using as a vector rather than
// a point.
typedef Point Arrow;

#endif