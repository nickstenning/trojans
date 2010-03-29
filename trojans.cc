#include "particle.hh"
#include "simulator.hh"
#include "constants.hh"

#include <iostream>
#include <iomanip>
#include <cmath>

using namespace std;

int main (int argc, char * const argv [])
{
  if (argc != 2)
  {
    cerr << "Usage: trojans <outputdir>" << endl << endl;
    cerr << "Output will be multiple files. Run ./movie <outputdir> to see the result." << endl;
    return 1;

  }
  else
  {
    // Increase precision of double printers.
    cout << setprecision(9);
    cerr << setprecision(9);

    string outputDir(argv[1]);

    Simulator sim(outputDir);

    double Rjup = 5.2;
    double Vjup = 2.757;

    double trojAngle = (C::pi / 3.0);

    Particle sun("sun", 1.0, Point(), Arrow());
    Particle jup("jupiter", 0.001, Point(0, Rjup), Arrow(Vjup, 0)); // 2.757 AU/year is plausible.
    Particle tro("trojans", 0,
                 Point(Rjup * sin(trojAngle),  Rjup * cos(trojAngle)),
                 Arrow(Vjup * cos(trojAngle), -Vjup * sin(trojAngle)));

    sim.addParticle(sun);
    sim.addParticle(jup);
    sim.addParticle(tro);

    sim.run(25); // period of Jupiter is ~11.9 years, so this should be about
                 // two orbits.

    return 0;
  }
}