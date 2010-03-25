#include "particle.hh"
#include "constants.hh"

// #include <gsl/gsl_errno.h>
// #include <gsl/gsl_matrix.h>
// #include <gsl/gsl_odeiv.h>
#include <vector>
#include <string>

#ifndef SIMULATOR_HH_
#define SIMULATOR_HH_

class Simulator {
  public:
    Simulator(std::string outputDir);

    int addParticle(const Particle &p);
    Arrow computeAcceleration(const Particle &p);
    int run(double time);
    void step(double time, double dt);
    void printTrajectories();
  private:
    std::vector<Particle> particles;
    std::string outputDir;
};

#endif /* SIMULATOR_HH_ */
