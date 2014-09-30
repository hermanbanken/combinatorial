#include "structures.h"
#include <cstdlib>
#include <iostream>

using namespace std;

string point::toString() {
    char *r = (char *) malloc(100);
    sprintf(r,
            "(%f,%f)(dept=%f,seabed=%i,builder=%i,bomb=%i,pipeline=%i)\n",
            x, y, z, seabed, builder, bomb, pipeline
    );
    std::string str(r);
    return str;
}

void aggregationCell::addPoint(point *p) {
    double factor = ((double) this->count) / (this->count + 1);
    this->z = this->z * factor + p->z / (this->count + 1);

    this->builder = max(this->builder, p->builder);
    this->bomb = max(this->bomb, p->bomb);
    this->pipeline = max(this->pipeline, p->pipeline);

    this->addBed(p->seabed);
    this->count++;
}

void aggregationCell::addBed(SeaBed b) {
    if (b == Sand)
        this->beds[0]++;
    else if (b == Rock)
        this->beds[1]++;
    else if (b == Mud)
        this->beds[2]++;
    else
        cerr << "Unknown seabed [1]: " << b << " add this type and recompile.\n";
}

SeaBed aggregationCell::seabed() {
    if (this->beds[0] > this->beds[1] && this->beds[0] > this->beds[2])
        return Sand;
    else if (this->beds[1] > this->beds[2])
        return Rock;
    else
        return Mud;
}

cell aggregationCell::toCell() {
    cell c = cell();
    c.z = this->z;
    c.bomb = this->bomb;
    c.builder = this->builder;
    c.pipeline = this->pipeline;
    c.seabed = this->seabed();
    c.mapped = this->count > 0;
    return c;
}

point::point() {
}

point::point(float x, float y, float z) {
    this->x = x;
    this->y = y;
    this->z = z;
}
