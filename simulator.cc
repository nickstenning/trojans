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

int Simulator::run (double tMax)
{
  Params params;
  params.particles = &particles;

  const unsigned int degreesOfFreedom = 4 * particles.size();

  // Non-adaptive RK4 integration steps
  const gsl_odeiv_step_type* T = gsl_odeiv_step_rk4;
  gsl_odeiv_step* s = gsl_odeiv_step_alloc(T, degreesOfFreedom);

  gsl_odeiv_system sys = {func, jac, degreesOfFreedom, &params};

  double* y = NULL;
  double* y_err = NULL;
  double* dydt_in = NULL;
  double* dydt_out = NULL;

  y        = new double[degreesOfFreedom];
  y_err    = new double[degreesOfFreedom];
  dydt_in  = new double[degreesOfFreedom];
  dydt_out = new double[degreesOfFreedom];

  for(size_t idx = 0; idx < particles.size(); ++idx)
  {
    Arrow vel = particles[idx]->getVelocity();
    Point pos = particles[idx]->getPosition();
    y[idx + v_x] = vel.x;
    y[idx + v_y] = vel.y;
    y[idx + r_x] = pos.x;
    y[idx + r_y] = pos.y;
  }

  double t  = 0.0;
  double dt = 1.0e-2;

  while (t < tMax)
  {
    int status = gsl_odeiv_step_apply(s, t, dt, y, y_err, dydt_in, dydt_out, &sys);

    if (status != GSL_SUCCESS) { break; }

    for (ParticleList::iterator p = particles.begin(); p != particles.end(); ++p)
    {
      ParticleList::difference_type idx = p - particles.begin();
      (*p)->setVelocity(Arrow(y[idx + v_x], y[idx + v_y]));
      (*p)->setPosition(Point(y[idx + r_x], y[idx + r_y]));
      (*p)->printDataLine(t, particles);
    }

    t += dt;
  }

  gsl_odeiv_step_free(s);

  delete [] y;
  delete [] y_err;
  delete [] dydt_in;
  delete [] dydt_out;
  y = y_err = dydt_in = dydt_out = NULL;

  return 0;
}

ostream& operator<< (ostream &os, const Simulator& obj)
{
  os << "<Simulator particles:" << obj.particles.size() << ">";
  return os;
}

int func (double /*t*/, const double y [], double dy_dt [], void* params) {
  Params *p = (Params *) params;

  for(size_t idx = 0; idx < p->particles->size(); ++idx)
  {
    Arrow accel = p->particles->at(idx)->computeAcceleration(*(p->particles));

    // dv/dt = a --> dx_1/dt = accel
    dy_dt[idx + v_x] = accel.x;
    dy_dt[idx + v_y] = accel.y;

    // dx/dt = v --> dx_0/dt = x_1
    dy_dt[idx + r_x] = y[idx + v_x];
    dy_dt[idx + r_y] = y[idx + v_y];
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