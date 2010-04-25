#include "simulator.hh"

using std::cout;
using std::cerr;
using std::endl;

char const Simulator::lockFileName[] = "lock";

Simulator::Simulator(std::string od) : outputDir(od) {
  // Append slash to outputDir if not provided.
  if (outputDir.compare(outputDir.length() - 1, 1, "/") != 0) {
    outputDir.append("/");
  }

  system(("mkdir -p " + outputDir).c_str());

  std::string lockFile = outputDir + lockFileName;

  if (access(lockFile.c_str(), F_OK) != -1) {
    cerr << "Refusing to proceed while lockfile '" << lockFile << "' exists: exiting!" << endl;
    exit(1);
  }
}

Simulator::~Simulator() {
  if (dataFile.is_open()) { dataFile.close(); }

  for (std::vector<std::ofstream*>::iterator f = particleDataFiles.begin();
       f != particleDataFiles.end(); ++f) {
    if ((*f)->is_open()) { (*f)->close(); }
    delete *f;
  }
}

int Simulator::addParticle(Particle const& particle) {
  particles.push_back(particle);
  return particles.size();
}

void Simulator::updateParticle(Particle& particle) {
  double energy;
  Arrow acceleration, distance;

  for (Particles::const_iterator p = particles.begin(); p != particles.end(); ++p) {
    // Don't calculate acceleration due to this particle.
    if (*p != particle) {
      distance = p->position() - particle.position();

      energy += (C::G_scaled * p->mass() * particle.mass()) / distance.norm();
      acceleration += (C::G_scaled * p->mass() * distance) / pow(distance.norm(), 3);
    }
  }

  // KE
  energy += (particle.mass() * particle.velocity().normsq()) / 2.0;

  particle.acceleration(acceleration);
  particle.energy(energy);
}

int Simulator::run(double tMax, size_t numFrames, void (*onFrameFunc)(size_t)) {
  openDataFiles();

  Params par;
  par.particles = &particles;

  size_t dof = degreesOfFreedom();

  // Embedded Runge-Kutta Prince-Dormand (8, 9) stepping function
  gsl_odeiv_step* s = gsl_odeiv_step_alloc(gsl_odeiv_step_rk8pd, dof);

  // Embedded Runge-Kutta-Fehlberg (4, 5) stepping function
  // gsl_odeiv_step* s = gsl_odeiv_step_alloc(gsl_odeiv_step_rkf45, dof);

  gsl_odeiv_control* c = gsl_odeiv_control_y_new(1e-20, 0);
  gsl_odeiv_evolve* e = gsl_odeiv_evolve_alloc(dof);

  gsl_odeiv_system sys = {func, NULL, dof, &par};

  double *y = NULL;
  y = new double[dof];

  t  = 0.0;
  dt = 1e-6;

  for (Particles::iterator p = particles.begin(); p != particles.end(); ++p) {
    updateParticle(*p);
  }
  setArrayFromParticles(y);
  // Print t = 0 frame.
  printData();

  for (size_t frame = 0; frame < numFrames - 1; ++frame) {
      double tFrame = (frame + 1) * (tMax / numFrames);

      while (t < tFrame) {
        int status = gsl_odeiv_evolve_apply(e, c, s, &sys, &t, tFrame, &dt, y);

        if (status != GSL_SUCCESS) {
          return status;
        }

        setParticlesFromArray(y);
      }

      printData();
      onFrameFunc(frame);
  }

  gsl_odeiv_step_free(s);
  gsl_odeiv_evolve_free(e);
  gsl_odeiv_control_free(c);

  delete [] y;
  y = NULL;

  createLockFile();
  return 0;
}

size_t Simulator::degreesOfFreedom() const {
  return dofParticle * particles.size();
}

std::ostream& operator<<(std::ostream &os, Simulator const& s) {
  os << "<Simulator particles:[\n";
  for (Particles::const_iterator p = s.particles.begin(); p != s.particles.end(); ++p) {
    os << "  " << *p << ",\n";
  }
  os << "]>";
  return os;
}

void Simulator::setArrayFromParticles(double y[]) {
  for (Particles::const_iterator p = particles.begin(); p != particles.end(); ++p) {
    Particles::difference_type idx = p - particles.begin();

    y[ypos(idx, v_x)] = p->velocity().x;
    y[ypos(idx, v_y)] = p->velocity().y;
    y[ypos(idx, r_x)] = p->position().x;
    y[ypos(idx, r_y)] = p->position().y;
  }
}

void Simulator::setParticlesFromArray(double const y[]) {
  for (Particles::iterator p = particles.begin(); p != particles.end(); ++p) {
    Particles::difference_type idx = p - particles.begin();

    p->position(Point(y[ypos(idx, r_x)], y[ypos(idx, r_y)]));
    p->velocity(Arrow(y[ypos(idx, v_x)], y[ypos(idx, v_y)]));

    updateParticle(*p);
  }
}

void Simulator::printData() {
  double totalEnergy = 0.0;
  double totalMass = 0.0;
  Point barycenter;
  bool foundJupiter;
  Point jupPos;

  for (Particles::iterator p = particles.begin(); p != particles.end(); ++p) {
    Particles::difference_type idx = p - particles.begin();

    totalEnergy += p->energy();
    totalMass += p->mass();

    barycenter += p->position() * p->mass();

    if (p->name() == "jupiter") {
      foundJupiter = true;
      jupPos = p->position();
    }

    if (particleDataFiles.at(idx)->is_open()) {
      p->printData(t, *particleDataFiles.at(idx));
    }
  }

  barycenter /= totalMass;

  dataFile << t << "\t" << totalEnergy << "\t" << barycenter.x << "\t" << barycenter.y;

  if (foundJupiter) {
    dataFile << "\t" << (jupPos - barycenter).arg();
  }

  dataFile << "\n";
}

void Simulator::openDataFiles() {
  particleDataFiles.clear();

  for (Particles::const_iterator p = particles.begin(); p != particles.end(); ++p) {
    std::string fname(outputDir + p->name());
    std::ofstream* df = new std::ofstream(fname.c_str(), std::ios::out | std::ios::trunc);
    particleDataFiles.push_back(df);

    if (df->is_open()) {
      p->printHeader(*df);
      *df << std::setprecision(9);
    } else {
      cerr << "ERROR: Unable to open output file for writing (" << fname << ")!" << endl;
      exit(2);
    }
  }

  std::string fname(outputDir + "system");
  dataFile.open(fname.c_str(), std::ios::out | std::ios::trunc);

  if (dataFile.is_open()) {
    dataFile << "#t\tenergy\tbary_x\tbary_y\tjup_theta\n";
    dataFile << std::setprecision(9);
  } else {
    cerr << "ERROR: Unable to open output file for writing (" << fname << ")!" << endl;
    exit(2);
  }
}

void Simulator::createLockFile() {
  std::string lockFile = outputDir + lockFileName;
  std::ofstream lf(lockFile.c_str());
  lf << endl;
  lf.close();
}

size_t ypos(size_t index, ParticleProperty prop) {
  return Simulator::dofParticle * index + prop;
}

int func(double /*t*/, double const y[], double dy_dt[], void* params) {
  Params* par = static_cast<Params*>(params);

  for (Particles::const_iterator p = par->particles->begin(); p != par->particles->end(); ++p) {
    Particles::difference_type idx = p - par->particles->begin();

    // dv/dt = a --> dx_1/dt = accel
    dy_dt[ypos(idx, v_x)] = p->acceleration().x;
    dy_dt[ypos(idx, v_y)] = p->acceleration().y;

    // dx/dt = v --> dx_0/dt = x_1
    dy_dt[ypos(idx, r_x)] = y[ypos(idx, v_x)];
    dy_dt[ypos(idx, r_y)] = y[ypos(idx, v_y)];
  }

  return GSL_SUCCESS;
}