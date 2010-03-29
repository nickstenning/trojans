#ifndef POINT_HH_
#define POINT_HH_

#include <cmath>
#include <sstream>

class Point
{
public:
  Point ();
  Point (double xx, double yy);

  double norm() const;
  double normsq() const;

  Point& operator= (const Point& rhs);
  Point& operator+= (const Point& rhs);
  Point& operator-= (const Point& rhs);
  Point& operator*= (double rhs);
  Point& operator/= (double rhs);

  const Point operator* (double factor) const;
  const Point operator/ (double divisor) const;
  const Point operator+ (const Point& rhs) const;
  const Point operator- (const Point& rhs) const;

  friend std::ostream &operator<<(std::ostream& os, const Point& obj);

  double x, y;
};

// Allow "2 * Point(0, 4)" as well. Member function only
// allows "Point(0, 4) * 2".
const Point operator* (double factor, const Point& rhs);

// Syntactic sugar for when we're using as a vector rather than
// a point.
typedef Point Arrow;

#endif