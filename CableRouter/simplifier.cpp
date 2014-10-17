#include <iostream>
#include <math.h>
#include "structures.h"
#include "simplifier.h"
#include <limits.h>
#include <stdexcept>


Simplifier::Simplifier() {
    min = new point(INT_MAX, INT_MAX, INT_MAX);
    max = new point(INT_MIN, INT_MIN, INT_MIN);
    avg = new point();
}

int Simplifier::add(point *row) {
    this->data.push_back(row);

    this->min->x = (float) fmin(this->min->x, row->x);
    this->min->y = (float) fmin(this->min->y, row->y);
    this->min->z = (float) fmin(this->min->z, row->z);

    this->max->x = (float) fmax(this->max->x, row->x);
    this->max->y = (float) fmax(this->max->y, row->y);
    this->max->z = (float) fmax(this->max->z, row->z);

    float factor = ((float) this->count) / (this->count + 1);
    this->avg->x = (this->avg->x * factor) + row->x / (this->count + 1);
    this->avg->y = (this->avg->y * factor) + row->y / (this->count + 1);
    this->avg->z = (this->avg->z * factor) + row->z / (this->count + 1);
    this->count++;

    return true;
}

Grid* Simplifier::grid(float cell_w, float cell_h) {
    if(cell_w < 0 || cell_h < 0)
        throw invalid_argument("the grid dimensions must be at least 1");
    if(count == 0)
        throw invalid_argument("please first add cells to the simplifier before simplyfing to a grid");

    float grid_x = this->min->x;
    float grid_y = this->min->y;
    float grid_w = this->max->x - this->min->x;
    float grid_h = this->max->y - this->min->y;
    unsigned long count_x = (unsigned long) (grid_w / cell_w) + 1;
    unsigned long count_y = (unsigned long) (grid_h / cell_h) + 1;

    // Group points
    vector<vector<aggregationCell>> g = vector<vector<aggregationCell>>(count_x, vector<aggregationCell>(count_y, aggregationCell()));
    vector<point*>::const_iterator it;
    for(it = this->data.begin(); it < this->data.end(); it++){
        unsigned long x = (unsigned long) floor(((*it)->x - grid_x) / cell_w);
        unsigned long y = (unsigned long) floor(((*it)->y - grid_y) / cell_h);
        g.at(x).at(y).addPoint(*it);
    }

    cout << "Initialized grid vector with " << count_x << " x " << count_y << " cells." << endl << flush;
    // Aggregate
    vector<vector<cell>> r = vector<vector<cell>>(count_x, vector<cell>(count_y, cell()));
    for(unsigned int x = 0; x < g.size(); x++){
        for(unsigned int y = 0; y < g[x].size(); y++){
            r.at(x)[y] = g[x][y].toCell();
        }
    }

    return new Grid(r, Projection(-grid_x, -grid_y, 1.0f/cell_w, 1.0f/cell_h));
}