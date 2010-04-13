#include "particle.hh"
#include "simulator.hh"

using namespace std;

Particle::Particle () {}

Particle::Particle (string n, const double m, 
                    Point r = Point(), Arrow v = Arrow(), 
                    bool f = false)
: name_(n), mass_(m), position_(r), velocity_(v), fixed_(f)
{}

string const& Particle::name () const { return name_; }

void Particle::name (const string& newName) {
  name_ = newName;
}

double const& Particle::mass () const { return mass_; }

void Particle::mass (const double& newMass) {
  mass_ = newMass;
}

double const& Particle::energy () const { return energy_; }

void Particle::energy (const double& newEnergy) {
  energy_ = newEnergy;
}

Point const& Particle::position () const { return position_; }

void Particle::position (const Point& newPosition) {
  if (fixed_) { return; }
  position_ = newPosition;
}

Arrow const& Particle::velocity () const { return velocity_; }

void Particle::velocity (Arrow const& newVelocity) {
  if (fixed_) { return; }
  velocity_ = newVelocity;
}

Arrow const& Particle::acceleration () const { return acceleration_; }

void Particle::acceleration (Arrow const& newAcceleration) {
  if (fixed_) { return; }
  acceleration_ = newAcceleration;
}

bool const& Particle::fixed () const { return fixed_; }

bool Particle::operator== (Particle const& rhs) const {
  return (name_ == rhs.name_);
}

bool Particle::operator!= (Particle const& rhs) const {
  return !(*this == rhs);
}

void Particle::printHeader (ofstream& ofs) const {  
  ofs << "# " << name_ << endl;
  
  if (fixed_) {
    ofs << "# r_x\tr_y";
  } else {
    ofs << "# t\tr_x\tr_y\tv_x\tv_y\ta_x\ta_y\tenergy";
  }

  ofs << endl;
}

void Particle::printData (const double& t, ofstream& ofs) const { 
  if (fixed_) {
    ofs << position_.x << "\t" << position_.y << endl;
    ofs.close(); // Won't come round again.
  } else {
    ofs << t << "\t"
        << position_.x << "\t" << position_.y << "\t"
        << velocity_.x << "\t" << velocity_.y << "\t"
        << acceleration_.x << "\t" << acceleration_.y << "\t"
        << energy_ << endl;
  }
}

ostream& operator<< (ostream& os, const Particle& p) {
  os << "<Particle " << p.name_;
  os << " m:" << p.mass_;
  os << " r:" << p.position_;
  if (p.fixed_) { 
    os << " fixed";
  } else {
    os << " v:" << p.velocity_;
  }
  os << ">";
  return os;
}

istream& operator>> (istream& is, Particle& p) {
  is >> boolalpha
     >> p.name_ >> p.mass_
     >> p.position_.x >> p.position_.y
     >> p.velocity_.x >> p.velocity_.y
     >> p.fixed_;
  return is;
}