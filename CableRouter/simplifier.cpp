#include <iostream>
#include <math.h>
#include "structures.h"
#include "simplifier.h"

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

vector<vector<cell>> *Simplifier::grid(int cell_w, int cell_h) {
    vector<vector<cell>> grid;

    /*
    double grid_x = min.x;
    double grid_y = min.y;
    double grid_w = max.x - min.x;
    double grid_h = max.y - min.y;
    int count_x = grid_w / GRID_CELL_W;
    int count_y = grid_h / GRID_CELL_H;

    // Define grid
    vector<vector<aggregationCell> > grid (count_x, vector<aggregationCell>(count_y, NULL));

    // Snap to grid
    int s = data.size();
    for (int i = 0; i < s; i++) {
        addToGrid(
                &grid,
                grid_x, grid_y, grid_w, grid_h,
                GRID_CELL_W, GRID_CELL_H,
                &data[i]
        );
    }
    */

    return &grid;

}