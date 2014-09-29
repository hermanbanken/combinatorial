#include <stdio.h>
#include <float.h>
#include <stdio.h>
#include <string>


#ifndef ____seabed_preprocess__
#define ____seabed_preprocess__

using namespace std;

enum SeaBed {
    Sand, Rock, Mud
};

struct cell {
    double z;
    bool builder;
    bool bomb;
    bool pipeline;
    SeaBed seabed;
};

struct point {
    point(float x, float y, float z);

    point();

    float x;
    float y;
    float z;
    SeaBed seabed;
    bool builder;
    bool bomb;
    bool pipeline;

    string toString();
};

struct aggregationCell {
    int count = 0;
    double z = 0;
    bool builder;
    bool bomb;
    bool pipeline;
    int beds[3]; // SeaBed.count

    void addPoint(point *p);

    void addBed(SeaBed b);

    SeaBed seabed();

};

#endif /* defined(____seabed_preprocess__) */