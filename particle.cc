#include "particle.hh"

using namespace std;

Particle::Particle(string n, const double m, Point x0, Arrow v0) : name(n), mass(m), x(x0), v(v0) {}

bool Particle::operator==(const Particle &rhs) const {
  return (this->name == rhs.name);
}

bool Particle::operator!=(const Particle &rhs) const {
  return !(*this == rhs);
}

ostream &operator<<(ostream &os, const Particle &obj) {
  os << "<Particle " << obj.name << ">";
  return os;
}