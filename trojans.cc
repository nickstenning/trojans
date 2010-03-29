#include "particle.hh"
#include "simulator.hh"

#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>

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

    FixedParticle sun("sun", 1.0, Point(0,0));
    Particle jup("jupiter", 0.001, Point(0,5.2), Arrow(2.757,0)); // 2.757 AU/year is plausible.

    sim.addParticle(sun);
    sim.addParticle(jup);

    sim.run(23.8); // period of Jupiter is ~11.9 years, so this should be about
                   // two orbits.

    return 0;
  }
}