#include "simulator.hh"

using namespace std;

Simulator::Simulator (string od) : outputDir(od)
{
  if (outputDir.compare(outputDir.length() - 1, 1, "/") != 0) {
    outputDir.append("/");
  }

  system(("mkdir -p " + outputDir).c_str());
}

Simulator::~Simulator () {
  if (dataFile.is_open()) { dataFile.close(); }
  
  for (vector<ofstream*>::iterator f = particleDataFiles.begin(); f != particleDataFiles.end(); ++f) {
    if ((*f)->is_open()) { (*f)->close(); }
    delete *f;
  } 
}

int Simulator::addParticle (Particle &p)
{
  particles.push_back(p);
  return particles.size();
}

void Simulator::run (double tMax, size_t numFrames,
                     void (*onFrameFunc)(size_t)) throw(string)
{
  openDataFiles();

  Params params;
  params.particles = &particles;


  size_t dof = degreesOfFreedom();

  // Embedded Runge-Kutta Prince-Dormand (8, 9) stepping function
  gsl_odeiv_step* s = gsl_odeiv_step_alloc(gsl_odeiv_step_rk8pd, dof);

  // Embedded Runge-Kutta-Fehlberg (4, 5) stepping function
  // gsl_odeiv_step* s = gsl_odeiv_step_alloc(gsl_odeiv_step_rkf45, dof);

  gsl_odeiv_control* c = gsl_odeiv_control_y_new(1e-20, 0);
  gsl_odeiv_evolve* e = gsl_odeiv_evolve_alloc(dof);

  gsl_odeiv_system sys = {func, jac, dof, &params};

  double *y = NULL;
  y = new double[dof];

  setArrayFromParticles(y);

  t  = 0.0;
  dt = 1e-6;

  for (size_t frame = 0; frame < numFrames; ++frame) {

      double tFrame = frame * (tMax / numFrames);

      while (t < tFrame) {
        // setArrayFromParticles(y);

        int status = gsl_odeiv_evolve_apply(e, c, s, &sys, &t, tFrame, &dt, y);

        if (status != GSL_SUCCESS) {
          throw string("GSL failure.");
          break;
        }

        updateParticlesFromArray(y);
      }

      printData();
      onFrameFunc(frame);
  }

  gsl_odeiv_step_free(s);
  gsl_odeiv_evolve_free(e);
  gsl_odeiv_control_free(c);

  delete [] y;
  y = NULL;
}

size_t Simulator::degreesOfFreedom () const
{
  return dofParticle * particles.size();
}

ostream& operator<< (ostream &os, const Simulator& obj)
{
  os << "<Simulator particles:[" << endl;
  for (ParticleConstIterator p = obj.particles.begin(); p != obj.particles.end(); ++p) {
    os << "  " << *p << "," << endl;
  }
  os << "]>";
  return os;
}

void Simulator::setArrayFromParticles(double y [])
{
  for (ParticleConstIterator p = particles.begin(); p != particles.end(); ++p) {
    ParticleList::difference_type idx = p - particles.begin();
    Arrow vel = p->getVelocity();
    Point pos = p->getPosition();
    y[ypos(idx, v_x)] = vel.x;
    y[ypos(idx, v_y)] = vel.y;
    y[ypos(idx, r_x)] = pos.x;
    y[ypos(idx, r_y)] = pos.y;
  }
}

void Simulator::updateParticlesFromArray(const double y [])
{
  for (ParticleIterator p = particles.begin(); p != particles.end(); ++p) {
    ParticleList::difference_type idx = p - particles.begin();

    p->setPosition(
      Point(y[ypos(idx, r_x)], y[ypos(idx, r_y)])
    );
    p->setVelocity(
      Arrow(y[ypos(idx, v_x)], y[ypos(idx, v_y)])
    );

  }
}


void Simulator::printData ()
{
  double totalEnergy = 0.0;

  for (ParticleIterator p = particles.begin(); p != particles.end(); ++p) {
    ParticleList::difference_type idx = p - particles.begin();

    totalEnergy += p->computeEnergy(particles);
    
    if (particleDataFiles.at(idx)->is_open()) {
      p->printData(t, *particleDataFiles.at(idx));
    }
  }

  dataFile << t << "\t" << totalEnergy << endl;
}

void Simulator::openDataFiles ()
{
  particleDataFiles.clear();
  
  for (ParticleConstIterator p = particles.begin(); p != particles.end(); ++p) {
    string fname(outputDir + p->getName());
    ofstream* df = new ofstream(fname.c_str(), ios::out | ios::trunc);
    particleDataFiles.push_back(df);
    
    if (df->is_open()) {
      p->printHeader(*df);
    } else {
      cerr << "ERROR: Unable to open output file for writing (" << fname << ")!" << endl;
      exit(2);
    }
  }
  
  string fname(outputDir + "system");
  dataFile.open(fname.c_str(), ios::out | ios::trunc);

  if (dataFile.is_open()) {
    dataFile << "# system" << endl;
    dataFile << "# t\tenergy" << endl;
  } else {
    cerr << "ERROR: Unable to open output file for writing (" << fname << ")!" << endl;
    exit(2);
  }
}

size_t ypos (size_t index, ParticleProperty prop) {
  return Simulator::dofParticle * index + prop;
}

int func (double /*t*/, const double y [], double dy_dt [], void* params) {
  Params *p = (Params *) params;

  for(size_t idx = 0; idx < p->particles->size(); ++idx) {
    Arrow accel = p->particles->at(idx).computeAcceleration(*(p->particles));

    // dv/dt = a --> dx_1/dt = accel
    dy_dt[ypos(idx, v_x)] = accel.x;
    dy_dt[ypos(idx, v_y)] = accel.y;

    // dx/dt = v --> dx_0/dt = x_1
    dy_dt[ypos(idx, r_x)] = y[ypos(idx, v_x)];
    dy_dt[ypos(idx, r_y)] = y[ypos(idx, v_y)];
  }

  return GSL_SUCCESS;
}

int jac (double /*t*/, const double /*y*/[], double *df_dy, double df_dt[], void *params)
{
  Params *p = (Params *) params;

  size_t dof = Simulator::dofParticle * p->particles->size();

  gsl_matrix_view df_dy_mat = gsl_matrix_view_array(df_dy, dof, dof);
  gsl_matrix* m = &df_dy_mat.matrix;

  /* Most matrix elements are zero */
  gsl_matrix_set_zero(m);

  for(size_t idx = 0; idx < p->particles->size(); ++idx) {
    /* d(dr/dt)/dv = 1.0 */
    gsl_matrix_set(m, ypos(idx, r_x), ypos(idx, v_x), 1.0);
    gsl_matrix_set(m, ypos(idx, r_y), ypos(idx, v_y), 1.0);
  }

  fill(df_dt, df_dt + dof, 0.0);

  return GSL_SUCCESS;
}