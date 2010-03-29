#include "simulator.hh"

using namespace std;

Simulator::Simulator (string od) : outputDir(od)
{
  if (outputDir.compare(outputDir.length() - 1, 1, "/") != 0)
  {
    outputDir.append("/");
  }

  system(("mkdir -p " + outputDir).c_str());
}

Simulator::~Simulator () {
  for (ParticleList::iterator p = particles.begin(); p != particles.end(); ++p)
  {
    (*p)->closeDataFile();
  }
}

int Simulator::addParticle (Particle &p)
{
  p.openDataFile(outputDir);
  particles.push_back(&p);
  return particles.size();
}

void Simulator::run (double tMax)
{
  Params params;
  params.particles = &particles;

  size_t dof = degreesOfFreedom();

  // Non-adaptive RK4 integration steps
  gsl_odeiv_step* s = gsl_odeiv_step_alloc(gsl_odeiv_step_rk4, dof);

  gsl_odeiv_system sys = {func, jac, dof, &params};

  double *y, *y_err, *dydt_in, *dydt_out;
  y = y_err = dydt_in = dydt_out = NULL;

  y        = new double[dof];
  y_err    = new double[dof];
  dydt_in  = new double[dof];
  dydt_out = new double[dof];

  setArrayFromParticles(y);

  t  = 0.0;
  dt = 1.0e-3;
  numSteps = 0;

  GSL_ODEIV_FN_EVAL(&sys, t, y, dydt_in);

  while (t < tMax)
  {
    int status = gsl_odeiv_step_apply(s, t, dt, y, y_err, dydt_in, dydt_out, &sys);

    if (status != GSL_SUCCESS) { break; }

    updateParticlesFromArray(y);

    for(size_t idx = 0; idx < dof; ++idx) {
      dydt_in[idx] = dydt_out[idx];
    }

    ++numSteps;
    t += dt;
  }

  gsl_odeiv_step_free(s);

  delete [] y;
  delete [] y_err;
  delete [] dydt_in;
  delete [] dydt_out;
  y = y_err = dydt_in = dydt_out = NULL;
}

size_t Simulator::degreesOfFreedom () const
{
  return dofParticle * particles.size();
}

ostream& operator<< (ostream &os, const Simulator& obj)
{
  os << "<Simulator particles:" << obj.particles.size() << ">";
  return os;
}

void Simulator::setArrayFromParticles(double y [])
{
  for (ParticleConstIterator p = particles.begin(); p != particles.end(); ++p)
  {
    ParticleList::difference_type idx = p - particles.begin();
    Arrow vel = (*p)->getVelocity();
    Point pos = (*p)->getPosition();
    y[ypos(idx, v_x)] = vel.x;
    y[ypos(idx, v_y)] = vel.y;
    y[ypos(idx, r_x)] = pos.x;
    y[ypos(idx, r_y)] = pos.y;
  }
}

void Simulator::updateParticlesFromArray(const double y [])
{
  for (ParticleIterator p = particles.begin(); p != particles.end(); ++p)
  {
    ParticleList::difference_type idx = p - particles.begin();
    if (!(*p)->isFixed) {
      (*p)->setPosition(
        Point(y[ypos(idx, r_x)], y[ypos(idx, r_y)])
      );
      (*p)->setVelocity(
        Arrow(y[ypos(idx, v_x)], y[ypos(idx, v_y)])
      );
    }
    if (numSteps % 100 == 0) {
      // Data updated, print data lines to file.
      (*p)->printDataLine(t, particles);
    }
  }
}

size_t ypos (size_t index, ParticleProperty prop) {
  return Simulator::dofParticle * index + prop;
}

int func (double /*t*/, const double y [], double dy_dt [], void* params) {
  Params *p = (Params *) params;

  for(size_t idx = 0; idx < p->particles->size(); ++idx)
  {
    Arrow accel = p->particles->at(idx)->computeAcceleration(*(p->particles));

    // dv/dt = a --> dx_1/dt = accel
    dy_dt[ypos(idx, v_x)] = accel.x;
    dy_dt[ypos(idx, v_y)] = accel.y;

    // dx/dt = v --> dx_0/dt = x_1
    dy_dt[ypos(idx, r_x)] = y[ypos(idx, v_x)];
    dy_dt[ypos(idx, r_y)] = y[ypos(idx, v_y)];
  }

  return GSL_SUCCESS;
}

// int jac (double t, const double y[], double *dfdy, double dfdt[], void *params)
int jac (double, const double [], double*, double [], void* )
{
  // Params *p = (Params *) params;

  // Don't use jacobian at the moment.

  return GSL_SUCCESS;
}