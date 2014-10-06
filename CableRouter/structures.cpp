#include "structures.h"
#include <cstdlib>
#include <iostream>
#include <math.h>

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

void Grid::plot(ostream &stream) {
    std::vector<vector<cell>>::const_iterator i;
    std::vector<cell>::const_iterator j;
    for(i=grid.begin(); i!=grid.end(); ++i){
        for(j=i->begin(); j!=i->end(); ++j){
            if(j->pipeline)
                stream << "|";
            else if (j->bomb)
                stream << "b";
            else if (j->builder)
                stream << ".";
            else if(j->mapped)
                stream << " ";
            else
                stream << "*";
        }
        stream << "\n";
    }
}

Grid::Grid(vector<vector<cell>> grid, Projection fromInputToGrid) : gridProjection(fromInputToGrid) {
    this->grid = grid;
    this->gridProjection = fromInputToGrid;
}

void Projection::project(float x, float y, float &out_x, float &out_y) {
    projectX(x, out_x);
    projectY(y, out_y);
}

Projection::Projection(float offset_x, float offset_y, float scale_x, float scale_y) : offset_x(offset_x), offset_y(offset_y), scale_x(scale_x), scale_y(scale_y) {
    is_identity = scale_x == 1 && scale_y == 1 && offset_x == 0 && offset_y == 0;
    is_equalScale = scale_x == scale_y;
}

void Projection::projectX(float x, float &out_x) {
    out_x = (x + offset_x) * scale_x;
}
void Projection::projectY(float y, float &out_y) {
    out_y = (y + offset_y) * scale_y;
}

Projection Grid::to_ZeroToOne_Projection() {
    return Projection(minX(), minY(),1/(maxX()-minX()), 1/(maxY()-minY()));
}

Projection Projection::back() {
    return Projection(-offset_x*scale_x, -offset_y*scale_y, 1/scale_x, 1/scale_y);
}

float Grid::angle(float ax, float ay, float bx, float by, Projection &p) {
    if(!p.hasEqualScales()) {
        p.project(ax, ay, ax, ay);
        p.project(bx, by, bx, by);
    }
    if(!gridProjection.hasEqualScales()){
        gridProjection.project(ax,ay,ax,ay);
        gridProjection.project(bx,by,bx,by);
    }
    return (float) atan2(by - ay, bx - ax);
}

float Grid::cost(float ax, float ay, float bx, float by, Projection &p) {
    float val = 0;

    // Fix projection
    p.project(ax, ay, ax, ay);
    p.project(bx, by, bx, by);

    /* Distance */
    double distance = sqrt((ax - bx) * (ax - bx) + (ay - by) * (ay - by)) * COST_CABLE;
    val += distance;

    /* Obstacles and off map */
    for(double p = 0; p < distance; p++){
        double cx = ax + bx * p / distance * (ax < bx ? 1 : -1);
        double cy = ay + by * p / distance * (ay < by ? 1 : -1);

        // Off map
        if(cx < 0 || cx > this->grid.size())
            val = FLT_MAX;
        else if(cy < 0 || cy > this->grid.begin()->size())
            val = FLT_MAX;
        else {
            cell c = this->grid[(unsigned long)cx][(unsigned long)cy];
            if(!c.mapped)
                val = FLT_MAX;
            else {
                // obstacles
            }
        }
    }

    return val;
}

float Grid::cost(float angle) {
    return (float) (COST_ANGLE * pow(abs(angle/ALLOWED_ANGLE), COST_ANGLE_POW));
}

float Grid::distance(float ax, float ay, float bx, float by, Projection &p) {
    // Fix projection
    if(!p.isIdentity()) {
        p.project(ax, ay, ax, ay);
        p.project(bx, by, bx, by);
    }
    if(!gridProjection.isIdentity()){
        this->backToInputProjection().project(ax, ay, ax, ay);
        this->backToInputProjection().project(bx, by, bx, by);
    }
    return (float) sqrt((ax - bx) * (ax - bx) + (ay - by) * (ay - by));
}

Projection Grid::backToInputProjection() {
    return gridProjection.back();
}

Projection Grid::inputProjection() {
    return gridProjection;
}
