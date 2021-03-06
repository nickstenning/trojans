#ifndef PARTICLE_HH_
#define PARTICLE_HH_

#include "point.hh"
#include "constants.hh"

#include <cmath>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>

class Particle
{
public:
  Particle();
  Particle(std::string name, double mass, Point x0, Arrow v0);

  std::string const& name() const;
  void name(std::string const& newName);

  double const& mass() const;
  void mass(double const& newMass);

  double const& energy() const;
  void energy(double const& newEnergy);

  Point const& position() const;
  void position(Point const& newPosition);

  Arrow const& velocity() const;
  void velocity(Point const& newVelocity);

  Arrow const& acceleration() const;
  void acceleration(Point const& newAcceleration);

  bool operator==(Particle const& rhs) const;
  bool operator!=(Particle const& rhs) const;

  void printHeader(std::ofstream& ofs) const;
  void printData(double const& time, std::ofstream& ofs) const;

  friend std::ostream& operator<<(std::ostream& os, Particle const& p);
  friend std::istream& operator>>(std::istream& is, Particle& p);

protected:
  std::string name_;
  double mass_;
  double energy_;
  Point position_;
  Arrow velocity_;
  Arrow acceleration_;
};

typedef std::vector<Particle> Particles;

#endif

