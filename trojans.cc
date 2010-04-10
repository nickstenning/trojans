#include "particle.hh"
#include "simulator.hh"
#include "constants.hh"

#include <iostream>
#include <iomanip>
#include <cmath>

using namespace std;

/* radius of jupiter's orbit in a.u. */
double Rjup = 5.2;
/* tangential velocity of jupiter in a.u./year */
double Vjup = 2.757;
/* simulator run time */
double T = 5 * sqrt(pow(Rjup, 3));
/* number of frames to print */
size_t numFrames = 1000;

size_t frameStrSize;

void onframe(size_t frame);

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
    
    // Work out how long the field for frame number should be.
    stringstream tmp;
    tmp << numFrames;
    frameStrSize = tmp.str().size();
    
    // Create new simulator with output directory specified on the command line.
    Simulator sim(argv[1]);

    double trojAngle = (C::pi / 3.0);

    Particle sun("sun", 1.0, Point(), Arrow());
    sun.isFixed = true;
    
    Particle jup("jupiter", 0.001, Point(0, Rjup), Arrow(Vjup, 0)); // 2.757 AU/year is plausible.
    Particle tro("trojans", 0,
                 Point(Rjup * sin(trojAngle),  Rjup * cos(trojAngle)),
                 Arrow(Vjup * cos(trojAngle), -Vjup * sin(trojAngle)));
    Particle gre("greeks", 0,
                 Point(-Rjup * sin(trojAngle), Rjup * cos(trojAngle)),
                 Arrow( Vjup * cos(trojAngle), Vjup * sin(trojAngle)));


    sim.addParticle(sun);
    sim.addParticle(jup);
    sim.addParticle(tro);
    sim.addParticle(gre);

    try
    {
      cerr << sim << endl << endl;
      cerr << "Params: t = 0.." << T << ", numFrames = " << numFrames << endl << endl;
      cerr << "Simulator running ";
      
      sim.run(T, numFrames, *onframe);
      
      onframe(numFrames); // Last frame.
      
      cerr << endl;
    }
    catch( string str )
    {
      cerr << endl << "Exception raised: " << str << endl;
      return 1;
    }
    
    return 0;
  }
}

void onframe(size_t frame)
{
  if (frame % 10 == 0)
  {
    if (frame > 0)
    {
      for (size_t i = 0; i < ((2 * frameStrSize) + 3); ++i) { cerr << '\b'; }
    }

    cerr << "[" << setw(frameStrSize) << frame << "/" << setw(frameStrSize) << numFrames << "]" << flush;
  }
}