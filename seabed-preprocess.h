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

enum SeaBed {Sand, Rock, Mud};

struct cell {
  double z      ;
  bool builder  ;
  bool bomb     ;
  bool pipeline ;
  SeaBed seabed ;
} ;

struct point {
  double x;
  double y;
  double z;
  SeaBed seabed;
  bool builder;
  bool bomb;
  bool pipeline;
  
  string toString();
} ;

struct aggregationCell {
  double z      ;
  bool builder  ;
  bool bomb     ;
  bool pipeline ;
  int beds[3]   ; // SeaBed.count
  
  void addBed(SeaBed b);
  SeaBed seabed();

} ;

#endif /* defined(____seabed_preprocess__) */
