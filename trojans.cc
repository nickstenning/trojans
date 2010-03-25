#include "simulator.hh"
#include "particle.hh"

#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>

using namespace std;

int main (int argc, char * const argv []) {
  if (argc != 2) {
    cerr << "Usage: trojans <outputdir>" << endl << endl;
    cerr << "Output will be multiple files. Run ./movie <outputdir> to see the result." << endl;
    return 1;

  } else {
    // Increase precision of double printers.
    cout << setprecision(9);
    cerr << setprecision(9);

    string outputDir(argv[1]);

    Simulator sim(outputDir);

    Particle sun("sun", 1.0, Point(0,0), Arrow(0,0));
    Particle jup("jupiter", 0.001, Point(0,5.2), Arrow(2.757,0)); // 2.757 AU/year is plausible.
    Particle jup2("jupiter2", 0.001, Point(0,-5.2), Arrow(2.757,0)); // 2.757 AU/year is plausible.

    sim.addParticle(sun);
    sim.addParticle(jup);
    sim.addParticle(jup2);

    // sim.run();

    cout << jup << " acceleration: " << sim.computeAcceleration(jup) << endl;
    cout << jup2 << " acceleration: " << sim.computeAcceleration(jup2) << endl;
    cout << sun << " acceleration: " <<  sim.computeAcceleration(sun) << endl;

    //
    //
    //     cout << "Writing to " << outputDir << endl;
    //
    //     ofstream sunData((outputDir + "sun").c_str());
    //     ofstream jupData((outputDir + "jupiter").c_str());
    //
    //     string gnuplotHeader("# t\tx[0]\tx[1]\tv[0]\tv[1]\tT\tV\tT+V");
    //
    //     if (sunData.is_open() && jupData.is_open()) {
    //       // Sun has fixed coordinates.
    //       sunData << "# SUN" << endl;
    //       sunData << "0 0" << endl;
    //       // Trajectory files for others.
    //       jupData << "# JUPITER" << endl;
    //       jupData << gnuplotHeader << endl;
    //
    //       sunData.close();
    //       jupData.close();
    //     } else {
    //       cout << "ERROR: Unable to open output files for writing!" << endl;
    //       return 1;
    //     }

    return 0;
  }
}