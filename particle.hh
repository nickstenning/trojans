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

class Particle
{
public:
  Particle (std::string name, double mass, Point x0, Arrow v0);
  virtual ~Particle() {};

  virtual Point getPosition ();
  virtual void setPosition (const Point& pos);

  virtual Arrow getVelocity ();
  virtual void setVelocity (const Arrow& vel);

  virtual Arrow computeAcceleration (const ParticleList& particles);
  virtual double computeEnergy (const ParticleList& particles);

  virtual void openDataFile (const std::string outputDir);
  virtual void closeDataFile ();
  virtual void printDataLine (double time, const ParticleList& particles);

  virtual bool operator== (const Particle& rhs);
  virtual bool operator!= (const Particle& rhs);

  virtual std::string dataFileHeader() const;

  friend std::ostream& operator<< (std::ostream& os, const Particle& p);
protected:
  std::string name;
  double mass;
  Point r; // position vector
  Arrow v; // velocity vector

  std::ofstream dataFile;
};

class FixedParticle : public Particle
{
public:
  FixedParticle (std::string name, double mass, Point r0);

  virtual void setPosition (const Point& pos);
  virtual void setVelocity (const Arrow& vel);

  virtual Arrow computeAcceleration (const ParticleList& particles) const;
  virtual double computeEnergy (const ParticleList& particles) const;

  virtual void openDataFile (const std::string outputDir);
  virtual void printDataLine (double time, const ParticleList& particles);

  virtual std::string dataFileHeader() const;
};

#endif

