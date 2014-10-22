#include <stdio.h>
#include <iostream>
#include <fstream>
#include <cstdio>
#include <float.h>
#include <vector>
#include <tuple>
#include "../solver.h"
#include "../structures.h"
#include "cmaes.h"

using namespace std;
using namespace libcmaes;

namespace Solvers {

    class GA: public Solver {
    private:
        unsigned int points;
        Grid *grid;
        coordinate start;
        coordinate end;
        vector<coordinate> straightLine(coordinate start, coordinate end, unsigned long points);
    public:
        GA(unsigned int points);

        void preprocess(Grid *grid, double &time) {
            clock_t start = clock();
            grid->floodFindDistancesToEdge();
            time = double(clock() - start);
        }

        void solve(Grid* grid, vector<coordinate> &line, double &time);

    };

}