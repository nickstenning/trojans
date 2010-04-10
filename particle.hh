#ifndef PARTICLE_HH_
#define PARTICLE_HH_

#include "point.hh"
#include "constants.hh"

#include <cmath>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>

class Particle;

typedef std::vector<Particle*> ParticleList;
typedef ParticleList::iterator ParticleIterator;
typedef ParticleList::const_iterator ParticleConstIterator;

class Particle
{
public:
  Particle (std::string name, double mass, Point x0, Arrow v0);
  ~Particle() {};

  Point getPosition ();
  void setPosition (const Point& pos);

  Arrow getVelocity ();
  void setVelocity (const Arrow& vel);

  Arrow computeAcceleration (const ParticleList& particles);
  double computeEnergy (const ParticleList& particles);

  void openDataFile (const std::string outputDir);
  void closeDataFile ();
  void printDataLine (double time, const ParticleList& particles);

  bool operator== (const Particle& rhs);
  bool operator!= (const Particle& rhs);

  std::string dataFileHeader() const;

  friend std::ostream& operator<< (std::ostream& os, const Particle& p);

  bool isFixed;
  double lastComputedEnergy;
  Arrow lastComputedAccel;
protected:
  std::string name;
  double mass;
  Point r; // position vector
  Arrow v; // velocity vector

  std::ofstream dataFile;
};
#endif

