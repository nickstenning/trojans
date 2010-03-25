#include "simulator.hh"

using namespace std;

Simulator::Simulator(string od) : outputDir(od) {
  if (outputDir.compare(outputDir.length() - 1, 1, "/") != 0) {
    outputDir.append("/");
  }

  // system(("mkdir -p " + outputDir).c_str());
}

int Simulator::addParticle(const Particle &p) {
  particles.push_back(p);
  return particles.size();
}

Arrow Simulator::computeAcceleration(const Particle &particle) {
  Arrow accel;

  for (vector<Particle>::iterator p = particles.begin(); p != particles.end(); ++p) {
    if (*p != particle) { // Don't calculate acceleration due to yourself!
      Arrow distance = p->x - particle.x;
      accel += (C::G_scaled * p->mass * distance) / pow(distance.norm(), 3);
    }
  }

  return accel;
}

int Simulator::run(double time) {
  double timestep = 1;

  for (double t = 0; t < time; t += timestep) {
    step(t, timestep);
    printTrajectories();
  }

  return 0;
}

void Simulator::step(double t, double dt) {
  cout << t << ": stepping " << dt << endl;
}

void Simulator::printTrajectories() {}