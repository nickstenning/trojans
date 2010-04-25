#include "particle.hh"
#include "simulator.hh"

using namespace std;

Particle::Particle () {}

Particle::Particle (string n, const double m,
                    Point r = Point(), Arrow v = Arrow())
: name_(n), mass_(m), position_(r), velocity_(v)
{}

string const& Particle::name () const { return name_; }

void Particle::name (string const& newName) {
  name_ = newName;
}

double const& Particle::mass () const { return mass_; }

void Particle::mass (double const& newMass) {
  mass_ = newMass;
}

double const& Particle::energy () const { return energy_; }

void Particle::energy (double const& newEnergy) {
  energy_ = newEnergy;
}

Point const& Particle::position () const { return position_; }

void Particle::position (Point const& newPosition) {
  position_ = newPosition;
}

Arrow const& Particle::velocity () const { return velocity_; }

void Particle::velocity (Arrow const& newVelocity) {
  velocity_ = newVelocity;
}

Arrow const& Particle::acceleration () const { return acceleration_; }

void Particle::acceleration (Arrow const& newAcceleration) {
  acceleration_ = newAcceleration;
}

bool Particle::operator== (Particle const& rhs) const {
  return (name_ == rhs.name_);
}

bool Particle::operator!= (Particle const& rhs) const {
  return !(*this == rhs);
}

void Particle::printHeader (ofstream& ofs) const {
  ofs << "#t\tr_x\tr_y\tv_x\tv_y\ta_x\ta_y\tenergy\n";
}

void Particle::printData (double const& t, ofstream& ofs) const {
  ofs << t << "\t"
      << position_.x << "\t" << position_.y << "\t"
      << velocity_.x << "\t" << velocity_.y << "\t"
      << acceleration_.x << "\t" << acceleration_.y << "\t"
      << energy_ << "\n";
}

ostream& operator<< (ostream& os, Particle const& p) {
  os << "<Particle " << p.name_
     << " m:" << p.mass_
     << " r:" << p.position_
     << " v:" << p.velocity_
     << ">";
  return os;
}

istream& operator>> (istream& is, Particle& p) {
  is >> p.name_ >> p.mass_
     >> p.position_.x >> p.position_.y
     >> p.velocity_.x >> p.velocity_.y;
  return is;
}