#ifndef SIMULATOR_HH_
#define SIMULATOR_HH_

#include <gsl/gsl_errno.h>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_odeiv.h>
#include <vector>
#include <string>
#include <fstream>
#include <memory>

#include "point.hh"
#include "particle.hh"

enum ParticleProperty { r_x = 0, r_y = 1, v_x = 2, v_y = 3 };

struct Params {
  ParticleList* particles;
};

int func (double t, const double y[], double dy_dt[], void *params);
int jac (double t, const double y[], double *dfdy, double dfdt[], void *params);
size_t ypos (size_t index, ParticleProperty prop);

class Simulator
{
public:
  Simulator (std::string outputDir);
  ~Simulator ();

  int addParticle (Particle& p);

  void run (double tMax, 
            size_t numFrames, 
            void(*onFrameFunc)(size_t)) throw(std::string);

  friend class Particle;
  friend std::ostream& operator<< (std::ostream& os, const Simulator& s);

  static const size_t dofParticle = 4;
  virtual size_t degreesOfFreedom() const;
  
  void openDataFile ();
  void printDataLine ();
protected:
  ParticleList particles;
  std::string outputDir;
  std::ofstream dataFile;
  double t;
  double dt;

  void setArrayFromParticles(double y []);
  void updateParticlesFromArray(const double y []);
};

#endif /* SIMULATOR_HH_ */
