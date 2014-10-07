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
        int points;
        Grid *grid;
        tuple<unsigned long, unsigned long> start;
        tuple<unsigned long, unsigned long> end;
    public:
        GA(int points);

        void solve(Grid* grid, vector<tuple<unsigned long, unsigned long>> &line);
    };

}