#ifndef SIMULATOR_HH_
#define SIMULATOR_HH_

#include <gsl/gsl_errno.h>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_odeiv.h>
#include <vector>
#include <string>
#include <fstream>
#include <iomanip>

#include "point.hh"
#include "particle.hh"

enum ParticleProperty { r_x = 0, r_y = 1, v_x = 2, v_y = 3 };

struct Params {
  Particles const* particles;
};

int func(double t,  double const y[], double dy_dt[], void* params);
size_t ypos(size_t index, ParticleProperty prop);

class Simulator
{
public:
  Simulator(std::string outputDir);
  ~Simulator();

  int addParticle(Particle const& p);
  void updateParticle(Particle& p);

  int run (double tMax, size_t numFrames, void (*onFrameFunc)(size_t));

  friend std::ostream& operator<<(std::ostream& os, Simulator const& s);

  static const size_t dofParticle = 4;
  virtual size_t degreesOfFreedom() const;

  void openDataFiles();
  void printData();
  void createLockFile();
protected:
  Particles particles;
  std::string outputDir;
  std::ofstream dataFile;
  std::vector<std::ofstream*> particleDataFiles;
  double t;
  double dt;

  static char const lockFileName[];

  void setArrayFromParticles(double y []);
  void setParticlesFromArray(double const y []);
};

#endif /* SIMULATOR_HH_ */
