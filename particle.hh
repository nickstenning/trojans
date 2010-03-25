#include "point.hh"

#include <cmath>
#include <vector>
#include <string>
#include <iostream>

#ifndef PARTICLE_HH_
#define PARTICLE_HH_

class Particle {
  public:
    Particle(std::string name, double mass, Point x0, Arrow v0);

    bool operator==(const Particle &rhs) const;
    bool operator!=(const Particle &rhs) const;

    friend std::ostream &operator<<(std::ostream &os, const Particle &p);
    friend class Simulator;

  private:
    std::string name;
    double mass;
    Point x;
    Arrow v;
};

#endif
