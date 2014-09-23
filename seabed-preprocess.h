//
//  seabed-preprocess.h
//  
//
//  Created by Herman Banken on 23-09-14.
//
//

#ifndef ____seabed_preprocess__
#define ____seabed_preprocess__

#include <string>
#include <stdio.h>
#include <cstdio>
#include <stdlib.h>

using namespace std;

enum SeaBed {sand, rock, mud};

struct point {
  double x;
  double y;
  double z;
  SeaBed seabed;
  bool builder;
  bool bomb;
  bool pipeline;
  
  string toString() {
    char * r = (char*)malloc(100);
    sprintf(r,
      "(%f,%f)(dept=%f,seabed=%i,builder=%i,bomb=%i,pipeline=%i)\n",
      x, y, z, seabed, builder, bomb, pipeline
    );
    std::string str(r);
    return str;
  }
} ;

#endif /* defined(____seabed_preprocess__) */
