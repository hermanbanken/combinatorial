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
        vector<vector<cell>> grid;
        tuple<unsigned long, unsigned long> start;
        tuple<unsigned long, unsigned long> end;
    public:
        GA(int points);

        double cost(double ax, double ay, double bx, double by, float &angle);

        void solve(vector<vector<cell>> &grid, vector<tuple<unsigned long, unsigned long>> &line);
    };

}