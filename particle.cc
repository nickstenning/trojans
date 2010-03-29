#include "particle.hh"
#include "simulator.hh"

using namespace std;

// ============
// = Particle =
// ============

Particle::Particle (string n, const double m, Point r0 = Point(), Arrow v0 = Arrow())
: name(n), mass(m), r(r0), v(v0)
{}

Point Particle::getPosition () { return r; }

void Particle::setPosition (const Point& pos){ r = pos; }

Arrow Particle::getVelocity () { return v; }

void Particle::setVelocity (const Arrow& vel) { v = vel; }

Arrow Particle::computeAcceleration (const ParticleList& particles)
{
  Arrow accel, distance;

  for (ParticleList::const_iterator p = particles.begin(); p != particles.end(); ++p)
  {
    if (**p != *this) { // Don't calculate acceleration due to yourself!
      distance = (*p)->getPosition() - this->getPosition();
      accel += (C::G_scaled * (*p)->mass * distance) / pow(distance.norm(), 3);
    }
  }

  return accel;
}

double Particle::computeEnergy (const ParticleList& particles)
{
  double energy = 0.0;
  Arrow distance;

  // PE
  for (ParticleList::const_iterator p = particles.begin(); p != particles.end(); ++p)
  {
    if (**p != *this) { // Don't calculate acceleration due to yourself!
      distance = (*p)->getPosition() - this->getPosition();
      energy += (C::G_scaled * (*p)->mass * mass) / distance.norm();
    }
  }

  // KE
  energy += (mass * v.normsq()) / 2.0;

  return energy;
}

void Particle::openDataFile (const string outputDir)
{
  string fname(outputDir + name);
  dataFile.open(fname.c_str());

  if (dataFile.is_open()) {
    dataFile << "# " << name << endl;
    dataFile << dataFileHeader() << endl;
  } else {
    cerr << "ERROR: Unable to open output file for writing (" << fname << ")!" << endl;
    exit(2);
  }
}

void Particle::closeDataFile ()
{
  dataFile.close();
}

void Particle::printDataLine (double t, const ParticleList& particles)
{
  dataFile << t << "\t"
           << r.x << "\t" << r.y << "\t"
           << v.x << "\t" << v.y << "\t"
           << computeEnergy(particles) << endl;
}

bool Particle::operator== (const Particle& rhs)
{
  return (name == rhs.name);
}

bool Particle::operator!= (const Particle& rhs)
{
  return !(*this == rhs);
}

string Particle::dataFileHeader () const
{
  return "# t\tr_x\tr_y\tv_x\tv_y\tenergy";
}

ostream& operator<< (ostream &os, const Particle& obj)
{
  os << "<Particle " << obj.name << " r:" << obj.r << " v:" << obj.v << ">";
  return os;
}