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
        coord start;
        coord end;
        vector<coord> straightLine(coord start, coord end, unsigned long points);
    public:
        GA(unsigned int points);

        void solve(Grid* grid, vector<coord> &line);
    };

}