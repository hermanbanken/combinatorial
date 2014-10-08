#include <stdio.h>
#include <iostream>
#include <fstream>
#include <cstdio>
#include <float.h>
#include <vector>
#include <tuple>
#include "structures.h"

#ifndef ___cablerouter_solver___
#define ___cablerouter_solver___

using namespace std;

namespace Solvers {

    class Solver {
    public:
        virtual void solve(Grid *grid, vector<coord> &line) = 0;
    };

}

#endif /* defined(___cablerouter_solver___) */