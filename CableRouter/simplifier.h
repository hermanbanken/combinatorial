#include <stdio.h>
#include <iostream>
#include <fstream>
#include <cstdio>
#include <float.h>
#include <vector>
#include "structures.h"

/**
* Aggregates points
*/
class Simplifier {
private:
    vector<point *> data = vector<point *>(0);
public:
    Simplifier();

    point *min;
    point *max;
    point *avg;
    int count = 0;

    /**
    * Update aggregations
    */
    int add(point *row);

    /**
    * Generate a simplified grid with the given cell widths and heights
    */
    vector<vector<cell>> grid(int cell_w, int cell_h);
};