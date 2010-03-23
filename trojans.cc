#include "constants.hh"

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
    if (outputDir.compare(outputDir.length() - 1, 1, "/") != 0) {
      outputDir.append("/");
    }
    
    system(("mkdir -p " + outputDir).c_str());
    
    cout << "Writing to " << outputDir << endl;
    
    ofstream sunData((outputDir + "sun").c_str());
    ofstream jupData((outputDir + "jupiter").c_str());
    
    string gnuplotHeader("# t\tx[0]\tx[1]\tv[0]\tv[1]\tT\tV\tT+V");
    
    if (sunData.is_open() && jupData.is_open()) {
      // Sun has fixed coordinates.
      sunData << "# SUN" << endl;
      sunData << "0 0" << endl;
      // Trajectory files for others.
      jupData << "# JUPITER" << endl;
      jupData << gnuplotHeader << endl;
            
      sunData.close();
      jupData.close();
    } else {
      cout << "ERROR: Unable to open output files for writing!" << endl;
      return 1;
    } 

    return 0;
  }
}