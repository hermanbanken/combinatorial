#include "structures.h"
#include <cstdlib>
#include <iostream>
#include <math.h>
#include <stdio.h>

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
    vector<vector<cell>>::const_iterator i;
    vector<cell>::const_iterator j;
    stream << "Outputting grid(" << grid.size() << "x" << grid.begin()->size() << "):" << endl;
    for(i=grid.begin(); i<grid.end(); i++){
        for(j=i->begin(); j<i->end(); j++) {
            if (j->pipeline)
                stream << "|";
            else if (j->bomb)
                stream << "b";
            else if (j->builder)
                stream << ".";
            else if (j->mapped)
                stream << " ";
            else {
                int d = distanceToMap((unsigned long) (i-grid.begin()), (unsigned long) (j-i->begin()));
                if(d > 9)
                    stream << "*";
                else
                    stream << d;
            }
        }
        stream << "\n" << flush;
    }
}

Grid::Grid(vector<vector<cell>> grid, Projection fromInputToGrid) : gridProjection(fromInputToGrid), grid(grid) {
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

Projection* Grid::to_ZeroToOne_Projection() {
    return new Projection(minX(), minY(),1/(maxX()-minX()), 1/(maxY()-minY()));
}

Projection* Projection::back() {
    return new Projection(-offset_x*scale_x, -offset_y*scale_y, 1/scale_x, 1/scale_y);
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
    double distance = COST_CABLE * this->distance(ax, ay, bx, by, *(Projection::identity()));
    val += distance;

    double grid_distance = (float) sqrt((ax - bx) * (ax - bx) + (ay - by) * (ay - by));

    /* Obstacles and off map */
    for(double p = 0; p < grid_distance; p++){
        double cx = ax + bx * p / grid_distance * (ax < bx ? 1 : -1);
        double cy = ay + by * p / grid_distance * (ay < by ? 1 : -1);

        // Off map
        if(cx < 0 || cx > grid.size())
            val = FLT_MAX;
        else if(cy < 0 || cy > grid.begin()->size())
            val = FLT_MAX;
        else {
            cell c = grid.at((unsigned long)cx).at((unsigned long)cy);
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
    return (float) (COST_ANGLE * pow(fabs(angle/ALLOWED_ANGLE), COST_ANGLE_POW));
}

float Grid::distance(float ax, float ay, float bx, float by, Projection &p) {
    // Fix projection
    if(!p.isIdentity()) {
        p.project(ax, ay, ax, ay);
        p.project(bx, by, bx, by);
    }
    if(!gridProjection.isIdentity()){
        this->backToInputProjection()->project(ax, ay, ax, ay);
        this->backToInputProjection()->project(bx, by, bx, by);
    }
    return (float) sqrt((ax - bx) * (ax - bx) + (ay - by) * (ay - by));
}

Projection* Grid::backToInputProjection() {
    return gridProjection.back();
}

Projection* Grid::inputProjection() {
    return &gridProjection;
}

void Grid::summary(ostream &stream) {
    stream << "Grid of " << grid.size() << "x" << grid[0].size() << "\n" << flush;
}

int Grid::distanceToMap(unsigned long x, unsigned long y, unsigned long origin_x, unsigned long origin_y, unsigned int maxRecurse) {
    if(origin_x - x > 10 || x - origin_x > 10 || origin_y - y > 10 || y - origin_y > 10){
        return INT8_MAX;
    }

    maxRecurse--;

    // Not even in grid, this is needed since very stupid recursive call
    if(x < 0 || y < 0 || x >= grid.size() || y >= grid[0].size())
        return INT8_MAX;

    cell* c = this->get(x, y);
    if(c->mapped){
        return 0;
    }

    return c->distanceToMap;

    if(maxRecurse <= 0)
        return INT8_MAX;

    if(x == origin_x && y == origin_y){
        c->distanceToMap = 1 + min(
                min(distanceToMap(x, y - 1, x, y, maxRecurse), distanceToMap(x, y + 1, x, y,maxRecurse)),
                min(distanceToMap(x - 1, y, x, y, maxRecurse), distanceToMap(x + 1, y, x, y,maxRecurse))
        );
    } else
    if(x == origin_x){
        c->distanceToMap = 1 + min(min(distanceToMap(x, y - 1, x, y,maxRecurse),
            distanceToMap(x, y + 1, x, y,maxRecurse)),
            distanceToMap(x + (x > origin_x ? 1 : -1), y, x, y,maxRecurse)
        );
    } else
    if(y == origin_y){
        c->distanceToMap = 1 + min(min(distanceToMap(x - 1, y, x, y,maxRecurse),
                        distanceToMap(x + 1, y, x, y,maxRecurse)),
                distanceToMap(x, y + (y > origin_y ? 1 : -1), x, y,maxRecurse)
        );
    } else {
        c->distanceToMap = 1 + min(
                distanceToMap(x + (x > origin_x ? 1 : -1), y, x, y,maxRecurse),
                distanceToMap(x, y + (y > origin_y ? 1 : -1), x, y,maxRecurse)
        );
    }

    return c->distanceToMap;
//        cout << "d(" << x << "," << y << ")=";
//        cout << c->distanceToMap << ";" << endl;
}

vector<pair<unsigned long, unsigned long>> Grid::edgeNodes() {
    vector<pair<unsigned long, unsigned long>> edges;
    vector<vector<cell>>::const_iterator x;
    vector<cell>::const_iterator c;
    for(x = grid.begin() + 1; x < grid.end() - 1; x++) {
        for (c = x->begin() + 1; c < x->end() - 1; c++) {
            unsigned long y = c - x->begin();
            if(
                    c->mapped != (x+1)->at(y).mapped ||
                    c->mapped != (x-1)->at(y).mapped ||
                    c->mapped != (c+1)->mapped ||
                    c->mapped != (c-1)->mapped
            ){
                edges.push_back(make_pair(x - grid.begin(), y));
            }
        }
    }
    return edges;
};

int Grid::distanceToMap(unsigned long x, unsigned long y) {
    return distanceToMap(x, y, x, y, 2);
}

void Grid::write(string filename) {
    FILE* pFile;
    pFile = fopen(filename.c_str(), "wb");

    fwrite(&gridProjection, sizeof(Projection), 1, pFile);

    vector<vector<cell>>::const_iterator i;
    vector<cell>::const_iterator j;
    unsigned long x_l = (unsigned long) (grid.end() - grid.begin());

    fwrite(&x_l, sizeof(unsigned long), 1, pFile);
    for(i = grid.begin(); i < grid.end(); i++){
        unsigned long y_l = (unsigned long) (i->end() - i->begin());
        fwrite(&y_l, sizeof(unsigned long), 1, pFile);
        for(j = i->begin(); j < i->end(); j++){
            fwrite(&j, sizeof(cell), 1, pFile);
        }
    }
    fclose(pFile);
}

Grid Grid::read(string filename) {
    FILE* f = fopen(filename.c_str(), "rb");
    Projection* p = new Projection(0,0,1,1);
    fread(p, sizeof(Projection), 1, f);

    unsigned long x_l = 0, y_l = 0;

    fread(&x_l, sizeof(unsigned long), 1, f);
    vector<vector<cell>> r = vector<vector<cell>>(x_l);
    for(unsigned long i = 0; i < x_l; i++){
        fread(&y_l, sizeof(unsigned long), 1, f);
        r.push_back(vector<cell>(y_l));
        cell row[y_l];
        fread(&row, sizeof(cell), y_l, f);
        vector<cell> v(y_l);
        for(unsigned long j = 0; j < y_l; j++)
            v.push_back(row[i]);
        r.push_back(v);
    }

    fclose(f);
    return Grid(&r, *p);
}

cell* Grid::getCell(float x, float y, Projection &p) {
    // Fix projection
    if(!p.isIdentity()) {
        p.project(x, y, x, y);
    }
    return this->get((unsigned long) x, (unsigned long) y);
}

cell* Grid::get(unsigned long x, unsigned long y) {
    if(x < 0 || y < 0 || x >= grid.size() || y >= grid.at(x).size()){
        exit(2);
//        throw invalid_argument("parameters must match the grid");
    } else {
        return &grid.at(x).at(y);
    }
}