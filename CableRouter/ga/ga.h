#include <stdio.h>
#include <iostream>
#include <fstream>
#include <cstdio>
#include <float.h>
#include <vector>
#include <tuple>
#include "../solver.h"
#include "../structures.h"

using namespace std;

namespace Solvers {

    class GA: public Solver {
    public:
        void solve(vector<vector<cell>> &grid, vector<tuple<unsigned long, unsigned long>> &line);
    };

}