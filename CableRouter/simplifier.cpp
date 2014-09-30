#include <iostream>
#include <math.h>
#include "structures.h"
#include "simplifier.h"
#include <limits.h>

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

vector<vector<cell>> Simplifier::grid(int cell_w, int cell_h) {
    float grid_x = this->min->x;
    float grid_y = this->min->y;
    float grid_w = this->max->x - this->min->x;
    float grid_h = this->max->y - this->min->y;
    unsigned long count_x = (unsigned long) (grid_w / cell_w);
    unsigned long count_y = (unsigned long) (grid_h / cell_h);

    // Group points
    vector<vector<aggregationCell>> g = vector<vector<aggregationCell>>(count_x+1, vector<aggregationCell>(count_y+1, aggregationCell()));
    for(unsigned int i = 0; i < this->data.size(); i++){
        unsigned long x = (unsigned long) ((this->data[i]->x - grid_x) / cell_w);
        unsigned long y = (unsigned long) ((this->data[i]->y - grid_y) / cell_h);
        g.at(x).at(y).addPoint(this->data[i]);
    }

    // Aggregate
    vector<vector<cell>> r = vector<vector<cell>>(count_x+1, vector<cell>(count_y+1, cell()));
    for(unsigned int x = 0; x < g.size(); x++){
        for(unsigned int y = 0; y < g[x].size(); y++){
            r[x][y] = g[x][y].toCell();
        }
    }

    return r;
}