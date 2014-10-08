#include "structures.h"
#include <cstdlib>
#include <iostream>
#include <math.h>
#include <stdio.h>
#include <queue>

#define ABS(a) (a > 0 ? a : (-1 * a))

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
                stream << "*";
            }
        }
        stream << "\n" << flush;
    }
}

Grid::Grid(vector<vector<cell>> grid, Projection fromInputToGrid) : grid(grid), gridProjection(fromInputToGrid) {
}

void Projection::project(double x, double y, double &out_x, double &out_y)const {
    projectX(x, out_x);
    projectY(y, out_y);
}

Projection::Projection(float offset_x, float offset_y, float scale_x, float scale_y) : offset_x(offset_x), offset_y(offset_y), scale_x(scale_x), scale_y(scale_y) {
    is_identity = scale_x == 1 && scale_y == 1 && offset_x == 0 && offset_y == 0;
    is_equalScale = scale_x == scale_y;
}

void Projection::projectX(double x, double &out_x)const {
    out_x = (x + offset_x) * scale_x;
}
void Projection::projectY(double y, double &out_y)const {
    out_y = (y + offset_y) * scale_y;
}

Projection Grid::to_ZeroToOne_Projection() {
    return Projection(minX(), minY(),1/(maxX()-minX()), 1/(maxY()-minY()));
}

Projection Projection::back() {
    return Projection(-offset_x*scale_x, -offset_y*scale_y, 1/scale_x, 1/scale_y);
}

double Grid::angle(double ax, double ay, double bx, double by, const Projection &p) {
    if(!p.hasEqualScales()) {
        p.project(ax, ay, ax, ay);
        p.project(bx, by, bx, by);
    }
    if(!gridProjection.hasEqualScales()){
        gridProjection.project(ax,ay,ax,ay);
        gridProjection.project(bx,by,bx,by);
    }
    return atan2(by - ay, bx - ax);
}

double Grid::cost(double ax, double ay, double bx, double by, const Projection &p, bool gradient) {
    double val = 0;

    // Fix projection
    p.project(ax, ay, ax, ay);
    p.project(bx, by, bx, by);

    /* Distance */
    double distance = COST_CABLE * this->distance(ax, ay, bx, by, Projection::identity());
    val += distance;

    double grid_distance = sqrt((ax - bx) * (ax - bx) + (ay - by) * (ay - by));

    /* Obstacles and off map */
    for(double p = 0; p < grid_distance; p++){
        double cx = ax + bx * p / grid_distance * (ax < bx ? 1 : -1);
        double cy = ay + by * p / grid_distance * (ay < by ? 1 : -1);

        cell c;
        bool exists = tryGet(cx, cy, c);
        if(exists && c.mapped) {

            // Penalties
            if(c.bomb)
                val += 50;
            if(c.pipeline)
                val += 100;
            if(c.builder)
                val += 5;

        } else if(!gradient){
            // Either not in grid or off map
            return DBL_MAX;
        } else if(!c.mapped) {
            // In grid, of map
            val += pow(COST_OFFMAP, this->distanceToMap(cx, cy));
        } else {
            // Of grid
            val += pow(COST_OFFMAP, this->distance(cx, cy, (maxX() - minX())/2, (maxY() - minY())/2, Projection::identity()));
        }
    }

    return val;
}

double Grid::cost(double angle, bool gradient) {
    if(!gradient)
        return fabs(angle) - ALLOWED_ANGLE > 0 ? DBL_MAX : 0;
    return (COST_ANGLE * pow(fabs(angle/ALLOWED_ANGLE), COST_ANGLE_POW));
}


double Grid::distance(double ax, double ay, double bx, double by, const Projection &p) {
    // Fix projection
    if(!p.isIdentity()) {
        p.project(ax, ay, ax, ay);
        p.project(bx, by, bx, by);
    }
    if(!gridProjection.isIdentity()){
        this->backToInputProjection().project(ax, ay, ax, ay);
        this->backToInputProjection().project(bx, by, bx, by);
    }
    return sqrt((ax - bx) * (ax - bx) + (ay - by) * (ay - by));
}

Projection Grid::backToInputProjection() {
    return gridProjection.back();
}

const Projection& Grid::inputProjection() {
    return gridProjection;
}

void Grid::summary(ostream &stream) {
    stream << "Grid of " << grid.size() << "x" << grid[0].size() << "\n" << flush;
}

class CompareForFlood {
public:
    /**
    * Returns true if b has a higher priority than a
    */
    bool operator()(pair<cell,coord>& a, pair<cell,coord>& b){
        if(b.first.distanceExplored < a.first.distanceExplored)
            return true;
        return false;
    }
};

vector<pair<cell,coord>> Grid::edgeNodes() {
    vector<pair<cell,coord>> edges;
    vector<vector<cell>>::iterator x;
    vector<cell>::iterator c;
    for(x = grid.begin() + 1; x < grid.end() - 1; x++) {
        for (c = x->begin() + 1; c < x->end() - 1; c++) {
            unsigned long y = c - x->begin();

            if(c->mapped)
                c->distanceToMap = 0;

            if(
                    c->mapped != (x+1)->at(y).mapped ||
                    c->mapped != (x-1)->at(y).mapped ||
                    c->mapped != (c+1)->mapped ||
                    c->mapped != (c-1)->mapped
            ){
                edges.push_back(make_pair(*c, make_pair(x - grid.begin(), y)));
            }
        }
    }
    return edges;
};

int Grid::distanceToMap(unsigned long x, unsigned long y) {
    cell c;
    if(!tryGet(x, y, c)){
       return INT32_MAX;
    } else {
        c = get(x, y);
        if(c.mapped)
            return 0;
        else if(c.distanceToMap != -1)
            return c.distanceToMap;

        floodFindDistancesToEdge();
        if(get(x,y).distanceToMap == -1)
            get(x,y).distanceToMap = INT32_MAX;

        return distanceToMap(x, y);
    }
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
    return Grid(r, *p);
}

const cell& Grid::getCell(double x, double y, Projection &p) {
    // Fix projection
    if(!p.isIdentity()) {
        p.project(x, y, x, y);
    }
    return this->get((unsigned long) x, (unsigned long) y);
}

cell& Grid::get(unsigned long x, unsigned long y) {
    if(x < 0 || y < 0 || x >= grid.size() || y >= grid.at(x).size()){
        throw invalid_argument("parameters must match the grid");
    } else {
        return grid.at(x).at(y);
    }
}

bool Grid::tryGet(unsigned long x, unsigned long y, cell& out_cell) {
    if(x < 0 || y < 0 || x >= grid.size() || y >= grid.at(x).size()){
        return false;
    } else {
        out_cell = grid.at(x).at(y);
        return true;
    }
}

void Grid::floodFindDistancesToEdge() {
    clock_t start = clock();

    priority_queue<pair<cell,coord>, vector<pair<cell,coord>>,CompareForFlood> pq(CompareForFlood(), this->edgeNodes());

    while(!pq.empty()){
        pair<cell,coord> current = pq.top();

        unsigned long Ox = current.second.first;
        unsigned long Oy = current.second.second;

        float dist = ++get(Ox,Oy).distanceExplored;
        unsigned long last_x = UINT32_MAX;
        unsigned long last_y = UINT32_MAX;
        int identified_count = 0;

        // Go in circle
        for(float r = 0; r < 2*M_PI; r += M_PI / dist / 4){
            unsigned long x = (unsigned long) (round(cos(r)*dist) + Ox);
            unsigned long y = (unsigned long) (round(sin(r)*dist) + Oy);

            // Skip if coordinates are not new
            if(ABS(x - last_x) < 1 && ABS(y - last_y) < 1)
                continue;

            cell a;
            if(tryGet(x, y, a) && (a.distanceToMap == -1 || a.distanceToMap > (int)floor(dist))){
                // debug:
                // cout << "Explored " << x << "," << y << " current dist: " << a.distanceToMap << " new dist: " << floor(dist) << endl;
                get(x,y).distanceToMap = (int) floor(dist);
                identified_count++;
            }

            last_x = x;
            last_y = y;
        }

        if(identified_count == 0){
            pq.pop();
        // debug:
        // cout << "Popped cell " << current.second.first << "," << current.second.second << endl;
        }
        // debug:
        // else
        // cout << "Identified " << identified_count << " by mapped cell " << current.second.first << "," << current.second.second << endl;
    }

    clock_t stop = clock();
    cout << double(stop - start) / CLOCKS_PER_SEC << " seconds for distance flood" << endl;

}

