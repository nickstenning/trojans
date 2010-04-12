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

typedef std::vector<Particle> ParticleList;
typedef ParticleList::iterator ParticleIterator;
typedef ParticleList::const_iterator ParticleConstIterator;

class Particle
{

public:
  Particle ();
  Particle (const Particle& obj);
  Particle (std::string name, double mass, Point x0, Arrow v0, bool fixed);
  ~Particle ();

  Particle& operator= (const Particle& obj);

  Point getPosition () const;
  void setPosition (const Point& pos);

  Arrow getVelocity () const;
  void setVelocity (const Arrow& vel);

  Arrow computeAcceleration (ParticleList& particles);
  double computeEnergy (ParticleList& particles);

  void openDataFile (const std::string outputDir);
  void printDataLine (double time);

  bool operator== (const Particle& rhs);
  bool operator!= (const Particle& rhs);
  
  std::string dataFileHeader() const;

  friend std::ostream& operator<< (std::ostream& os, const Particle& p);
  friend std::istream& operator>> (std::istream& is, Particle& p);

  double lastComputedEnergy;
  Arrow lastComputedAccel;
protected:
  std::string name;
  double mass;
  Point r; // position vector
  Arrow v; // velocity vector
  bool isFixed;

  std::ofstream dataFile;
};
#endif

