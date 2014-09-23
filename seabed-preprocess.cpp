//
//  seabed-preprocess.cpp
//  
//
//  Created by Herman Banken on 23-09-14.
//
//

#include "seabed-preprocess.h"
#include <vector>
#include <stdio.h>
#include <iostream>
#include <math.h>
#include <fstream>
#include <cstdio>
#include <algorithm>
#include <float.h>

using namespace std;

// Read a row
bool cvt (
  point &row,
  std::string const &line, char delimiter,
  point &r_min,
  point &r_max,
  point &r_avg,
  int &count)
{
  float z;
  int d;
  int sea;
  int obst;
  
  // Parse line
  int result = sscanf(line.c_str(), "%lf %lf %f %i %i %i", &row.x, &row.y, &z, &d, &sea, &obst);
  if(result < 6)
    return 0;
  
  // Convert z + burial depth
  row.z = lroundf(z*100) + d*100;
  // Seabed
  row.seabed = static_cast<SeaBed>(sea-1);
  // Obstacles
  row.builder = obst == 1;
  row.bomb = obst == 2;
  row.pipeline = obst == 3;
  
  // Update aggregations
  r_min.x = min(r_min.x, row.x);
  r_min.y = min(r_min.y, row.y);
  r_min.z = min(r_min.z, row.z);
  
  r_max.x = max(r_max.x, row.x);
  r_max.y = max(r_max.y, row.y);
  r_max.z = max(r_max.z, row.z);
  
  double factor = ((double) count) / (count + 1);
  r_avg.x = (r_avg.x * factor) + row.x / (count + 1);
  r_avg.y = (r_avg.y * factor) + row.y / (count + 1);
  r_avg.z = (r_avg.z * factor) + row.z / (count + 1);
  count++;
  
  return true;
}

int main (int argc, char const *argv[])
{
  // Start timer
  clock_t start=clock();
  
  vector<point> data;
  string line;
  string filename = argc >= 1 ? argv[1] : "data.txt";
  int count = 0;
  point min = { FLT_MAX, FLT_MAX, INT_MAX };
  point max = { FLT_MIN, FLT_MIN, INT_MIN };
  point avg;
  
  // Read
  ifstream in(filename);
  point row;
  int c = 0;
  while (std::getline(in, line) && in.good())
    if(cvt(row, line, ' ', min, max, avg, count))
      data.push_back(row);
  
  cout<<"Read "<<data.size()<<" points\n";
  cout<<"Min:\n\t"<<min.toString()<<"\n";
  cout<<"Max:\n\t"<<max.toString()<<"\n";
  cout<<"Avg:\n\t"<<avg.toString()<<"\n";
  
  // Write
  // ....
  
  // End timer
  clock_t stop=clock();
  cout<<double(stop-start)/CLOCKS_PER_SEC << " seconds\n";
  return 0;
}