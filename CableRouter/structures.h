#include <stdio.h>
#include <float.h>
#include <stdio.h>
#include <string>


#ifndef ___cablerouter_structures___
#define ___cablerouter_structures___

using namespace std;

enum SeaBed : char {
    Sand, Rock, Mud
};

struct cell {
    bool mapped = false;
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

    cell toCell();

    SeaBed seabed();

};

#endif /* defined(___cablerouter_structures___) */