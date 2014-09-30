#include <math.h>
#include <sstream>
#include "reader.h"

using namespace std;

Reader::Reader(const string filename) {
    in.open(filename);
}

int Reader::next(point *row) {
    if (!in.good())
        return false;

    string line;
    std::getline(in, line);
    if (line.empty())
        return false;

    float z = 0;
    int d = 0;
    int sea = 0;
    int obst = 0;

// New way of parsing below. However, this is slower than sscanf.
// Evaluate later to maybe use the newer method
//
//  // Parse line
//  std::istringstream rin(line);
//  rin >> row->x >> row->y >> z >> d >> sea >> obst;
//  // Error checking here
//    if(false){
//        cerr << "Skipped line: " << line << "\n";
//        return false;
//    }

    int result = sscanf(line.c_str(), "%f %f %f %i.000 %i.000 %i.000", &row->x, &row->y, &z, &d, &sea, &obst);
    if (result < 6) {
        cerr << "Skipped line: " << line << "\n";
        return false;
    }

    // Convert z + burial depth
    row->z = lroundf(z * 100) + d * 100;
    // Seabed
    row->seabed = static_cast<SeaBed>(sea - 1);
    // Obstacles
    row->builder = obst == 1;
    row->bomb = obst == 2;
    row->pipeline = obst == 3;

    return true;
}