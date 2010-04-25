#include <iostream>
#include <cmath>

#include "particle.hh"
#include "simulator.hh"
#include "constants.hh"

using std::cin;
using std::cout;
using std::cerr;
using std::endl;

// simulator run time (number of orbits * keplerian orbital period)
double T = 1 * 11.8724228;
// number of frames to print
size_t numFrames = 10000;

size_t frameStrSize;

void onframe(size_t frame);

int main(int argc, char* const argv[]) {
  if (argc != 2) {
    cerr << "Usage: trojans <outputdir> < config" << endl << endl;
    cerr << "Output will be multiple files. "
         << "Run, for example, `./plot.sh <outputdir> movie` to see the result." << endl;
    return 1;

  } else {
    // Create new simulator with output directory as specified on the command line.
    Simulator sim(argv[1]);

    // Load particles from stdin
    std::string s;
    while (getline(cin, s)) {
      // ignore comments
      if (s[0] != '#') {
        Particle p;
        std::istringstream ss(s);
        if ( ss >> p ) {
          sim.addParticle(p);
        }
      }
    }

    // This is a bit of a nasty hack to work out how long the field
    // for frame number should be.
    std::ostringstream tmp;
    tmp << numFrames;
    frameStrSize = tmp.str().size();

    // Dump basic config data to stderr.
    cerr << sim << endl << endl;
    cerr << "Params: t = 0.." << T << ", numFrames = "
         << numFrames << "\n\n";
    cerr << "Simulator running ";

    // Run the simulation. This will call the 'onframe' function with the
    // frame number as its only argument after each frame.
    int status;
    status = sim.run(T, numFrames, *onframe);

    if (status == 0) {
      onframe(numFrames);  // Last frame;
      cerr << endl;
    } else {
      cerr << "\nGSL error: status " << status << "!" << endl;
      return status;
    }

    return 0;
  }
}

void onframe(size_t frame) {
  if (frame > 0) {
    for (size_t i = 0; i < ((2 * frameStrSize) + 3); ++i) { cerr << '\b'; }
  }

  cerr << "[" << std::setw(frameStrSize) << frame << "/"
       << std::setw(frameStrSize) << numFrames << "]";
}
